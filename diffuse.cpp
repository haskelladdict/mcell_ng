// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "diffuse.hpp"
#include "util.hpp"


int collide(const State& state, VolMol& mol, Vec3& disp) {
  for (const auto& mesh : state.get_Meshes()) {
    for (const auto& m : mesh->get_meshElements()) {
      Vec3 hitPoint;
      if (intersect(mol.pos(), disp, m.get(), &hitPoint) == 0) {
        Vec3 disp_rem = hitPoint - mol.pos();

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
          std::cout << "foo" << std::endl;
          double side = (disp_rem * m->n_norm()) > 0 ? -1 : 1;
          hitPoint += side * GEOM_EPSILON * m->n_norm();
        }
        mol.moveTo(hitPoint);
        return 1;
      }
    }
  }
  return 0;
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
