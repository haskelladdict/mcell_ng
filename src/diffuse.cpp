// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <cmath>

#include "diffuse.hpp"
#include "vector.hpp"


// collide determines the closes mesh element along the displacement vector
// (if any) and the corresponding hitpoint on the mesh
int collide(const State& state, VolMol& mol, geom::Vec3& disp) {

  geom::Vec3 hitPoint;
  geom::Vec3 disp_rem;

  const auto& mesh = state.get_mesh();
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

  if (m == mesh.end()) {
    return 0;
  }

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


bool diffuse(State& state, const MolSpecies& spec, VolMol& mol, double dt) {

  // compute displacement
  double scale = sqrt(4*spec.D()*dt);
  geom::Vec3 disp{scale * state.rng_norm(), scale * state.rng_norm(),
    scale * state.rng_norm()};

  // diffuse and collide until we're at the end of our diffusion step
  while (collide(state, mol, disp)) {}
  if (norm2(disp) > 0) {
    mol.moveTo(mol.pos() + disp);
  }
  return true;
}


// replay_incoming_mols adds all molecules from the incoming queue to the
// active one.
static void replay_incoming_mols(const geom::Tet& tet, TetMols& mols) {
  auto& in = mols[tet.ID].inMols;
  auto& active = mols[tet.ID].activeMols;
  for (auto&& m: in) {
    active.add(std::move(m));
  }
  in.clear();
}


// replay_outgoing_mols adds all molecules in the outgoing queue to the
// incoming queues of the respective neighboring tets
static void replay_outgoing_mols(const geom::Tet& tet, TetMols& mols) {
  auto& outgoing = mols[tet.ID].outMols;
  for (size_t i=0; i < outgoing.size(); ++i) {
    auto& out = outgoing[i];
    if (out.size() == 0) {
      continue;
    }
    size_t targetID = tet.t[i];
    assert(targetID != geom::Tet::unset);
    auto& target = mols[targetID].inMols;
    target.insert(target.end(), std::make_move_iterator(out.begin()),
      std::make_move_iterator(out.end()));
    out.clear();
  }
}


// process_tet propagates all events that happen within the tet (molecule
// diffusion, reaction)
bool process_tet(const geom::Tet& tet, const geom::Mesh& mesh, TetMols& mols) {
  replay_incoming_mols(tet, mols);



  replay_outgoing_mols(tet, mols);
  return true;
}
