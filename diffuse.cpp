// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <cmath>

#include "diffuse.hpp"
#include "util.hpp"


// collide determines the closes mesh element along the displacement vector
// (if any) and the corresponding hitpoint on the mesh
int collide(const State& state, VolMol& mol, Vec3& disp) {

  Vec3 hitPoint;
  Vec3 disp_rem;

  const auto& mesh = state.get_mesh();
  Mesh::const_iterator m = mesh.end();
  double disp_len2 = std::numeric_limits<float>::max();
  for (Mesh::const_iterator it = mesh.begin(); it != mesh.end(); ++it) {
    Vec3 hitPoint_tmp;
    if (intersect(mol.pos(), disp, *it, &hitPoint_tmp) == 0) {
      Vec3 rem = hitPoint_tmp - mol.pos();
      double rem_len2 = rem.norm2();
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
  disp = disp_rem - (2 * (disp_rem * m->n_norm())) * m->n_norm();

  // move slightly away from the triangle along the reflected ray.
  // If we happen to end our ray at hitpoint we move along the triangle
  // normal instead.
  if (disp.norm2() > GEOM_EPSILON_2) {
    double n = disp.norm();
    auto disp_n = (1.0 / n) * disp;
    hitPoint += GEOM_EPSILON * disp_n;
    disp = (n - GEOM_EPSILON) * disp_n;
  } else {
    double side = (disp_rem * m->n_norm()) > 0 ? -1 : 1;
    hitPoint += side * GEOM_EPSILON * m->n_norm();
  }
  mol.moveTo(hitPoint);
  return 1;
}


bool diffuse(State& state, const MolSpecies& spec, VolMol& mol, double dt) {

  // compute displacement
  double scale = sqrt(4*spec.D()*dt);
  Vec3 disp{scale * state.rng_norm(), scale * state.rng_norm(),
    scale * state.rng_norm()};

  // diffuse and collide until we're at the end of our diffusion step
  while (collide(state, mol, disp)) {}
  if (disp.norm2() > 0) {
    mol.moveTo(mol.pos() + disp);
  }
  return true;
}
