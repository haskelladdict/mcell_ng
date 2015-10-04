// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <cmath>
#include <iostream>

#include "GSL/array_view.h"

#include "diffuse.hpp"
#include "vector.hpp"

#if 0
// collide determines the closes mesh element along the displacement vector
// (if any) and the corresponding hitpoint on the mesh
static std::tuple<int, geom::Vec3> collide(const State& state, VolMol& mol,
  geom::Vec3& disp) {

  geom::Vec3 hitPoint;
  geom::Vec3 disp_rem;

  const auto& mesh = state.mesh();
  geom::Mesh::const_iterator m = mesh.end();
  double disp_len2 = std::numeric_limits<float>::max();
  for (geom::Mesh::const_iterator it = mesh.begin(); it != mesh.end(); ++it) {
    geom::Vec3 hitPoint_tmp;
    if (intersect(mol.pos(), disp, *it, &hitPoint_tmp) == 0) {
      geom::Vec3 rem = hitPoint_tmp - mol.pos();
      double rem_len2 = norm2(rem);
      if (rem_len2 < disp_len2) {
        hitPoint = hitPoint_tmp;
        disp_rem = rem;
        disp_len2 = rem_len2;
        m = it;
      }
    }
  }

  int status = 1;
  if (m == mesh.end()) {
    status = 0;
  }
  return std::make_tuple(status, hitPoint);
}
#endif
#if 0
  // reflect: Rr = Ri - 2 N (Ri * N)
  disp = disp_rem - (2 * (disp_rem * m->n_norm)) * m->n_norm;

  // move slightly away from the triangle along the reflected ray.
  // If we happen to end our ray at hitpoint we move along the triangle
  // normal instead.
  if (norm2(disp) > geom::EPSILON_2) {
    double n = norm(disp);
    auto disp_n = (1.0 / n) * disp;
    hitPoint += geom::EPSILON * disp_n;
    disp = (n - geom::EPSILON) * disp_n;
  } else {
    double side = (disp_rem * m->n_norm) > 0 ? -1 : 1;
    hitPoint += side * geom::EPSILON * m->n_norm;
  }
  mol.moveTo(hitPoint);
  return 1;
}
#endif

bool diffuse(State& state, const MolSpecies& spec, VolMol& mol, double dt) {
  // compute displacement
  double scale = sqrt(4*spec.D()*dt);
  geom::Vec3 disp{scale * state.rng_norm(), scale * state.rng_norm(),
    scale * state.rng_norm()};

  // diffuse and collide until we're at the end of our diffusion step
  //while (collide(state, mol, disp)) {}
  if (norm2(disp) > 0) {
    mol.moveTo(mol.pos() + disp);
  }
  return true;
}


// replay_incoming_mols adds all molecules from the incoming queue to the
// active one.
static void replay_incoming_mols(TetMolState& molState) {
  auto& active = molState.activeMols;
  for (auto&& m: molState.inMols) {
    active.add(std::move(m));
  }
  molState.inMols.clear();
}


// replay_outgoing_mols adds all molecules in the outgoing queue to the
// incoming queues of the respective neighboring tets
static void replay_outgoing_mols(const geom::Tet& tet, TetMolState& molState) {
  auto& outgoing = molState.outMols;
  for (size_t i=0; i < outgoing.size(); ++i) {
    auto& out = outgoing[i];
    if (out.size() == 0) {
      continue;
    }
    size_t targetID = tet.t[i];
    assert(targetID != geom::Tet::unset);
    auto& target = molState.inMols;
    target.insert(target.end(), std::make_move_iterator(out.begin()),
      std::make_move_iterator(out.end()));
    out.clear();
  }
}

static std::tuple<int, size_t, geom::Vec3> collide(
    VolMol& mol, const geom::Vec3& disp, const geom::TetMeshes& mesh) {
  geom::Vec3 hitPoint;
  geom::Vec3 disp_rem;
  double disp_len2 = std::numeric_limits<float>::max();
  const geom::MeshElement* closestMesh = nullptr;
  size_t faceID = 0;
  for (size_t i=0; i < mesh.size(); ++i) {
    const auto m = mesh[i];
    geom::Vec3 hitPoint_tmp;
    if (intersect(mol.pos(), disp, m, &hitPoint_tmp) == 0) {
      geom::Vec3 rem = hitPoint_tmp - mol.pos();
      double rem_len2 = norm2(rem);
      if (rem_len2 < disp_len2) {
        hitPoint = hitPoint_tmp;
        disp_rem = rem;
        disp_len2 = rem_len2;
        closestMesh = m;
        faceID = i;
      }
    }
  }

  int status = 1;
  if (closestMesh == nullptr) {
    status = 0;
  }
  return std::make_tuple(status, faceID, hitPoint);
}

// diffuse_new
static int diffuse_new(VolMol& mol, geom::Vec3& disp,
                       const geom::TetMeshes& mesh) {
  int status = 0;
  int faceID = 0;
  geom::Vec3 hitPoint;

  while (true) {
    std::tie(status, faceID, hitPoint) = collide(mol, disp, mesh);

    // We didn't hit a mesh. Move molecule to final position and then return.
    if (status == 0) {
      break;
    }

    geom::Vec3 disp_rem = hitPoint - mol.pos();
    mol.inFlight = true;
    mol.moveTo(hitPoint);

    const geom::MeshElement* hitMesh = mesh[faceID];
    if (hitMesh->prop == geom::MeshProp::reflective) {
      disp = disp_rem - (2 * (disp_rem * hitMesh->n_norm)) * hitMesh->n_norm;
      mol.dispRem = disp;
    } else if (hitMesh->prop == geom::MeshProp::transparent) {
      mol.dispRem = disp_rem;
      return faceID;
    }
  }

  // done diffusing in this tet
  mol.moveTo(mol.pos() + disp);
  mol.inFlight = false;
  return -1;
}

// process_tet propagates all events that happen within the tet (molecule
// diffusion, reaction)
bool process_tet(State& state, size_t tetID) {
  const geom::Mesh& mesh = state.mesh();
  const geom::Tet& tet = state.tets()[tetID];
  const SpeciesContainer& specs = state.species();
  TetMolState& molState = state.tetMols(tetID);
  double dt = state.dt();

  replay_incoming_mols(molState);

  geom::TetMeshes tetMeshes{&mesh[tet.m[0]], &mesh[tet.m[1]], &mesh[tet.m[2]],
                            &mesh[tet.m[3]]};

  for (auto& s : molState.activeMols) {
    size_t specID = s.first;
    VolMolContainer& mols = s.second;
    std::cout << "before " << mols.size() << "\n";
    double scale = sqrt(4 * specs[specID].D() * state.dt());
    for (auto& mol : mols) {
      geom::Vec3 disp{scale * state.rng_norm(), scale * state.rng_norm(),
                      scale * state.rng_norm()};
      int status = diffuse_new(*mol, disp, tetMeshes);
      if (status == -1) {
        continue;
      } else {
        molState.outMols[status].push_back(std::move(mol));
        continue;
      }
    }
    // garbage collect dead mols
    mols.erase(std::remove_if(mols.begin(), mols.end(), [](auto& p) {
                 return p == nullptr;
               }), mols.end());
    std::cout << "after " << mols.size() << "\n";
  }

  replay_outgoing_mols(tet, molState);
  return true;
}
