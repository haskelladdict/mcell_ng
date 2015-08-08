// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "diffuse.hpp"
#include "util.hpp"


int collide(const State& state, VolMol& mol, Vector3D& disp) {
  for (const auto& mesh : state.get_Meshes()) {
    for (const auto& m : mesh->get_meshElements()) {
      Vector3D hitPoint;
      if (intersect(mol.pos(), disp, m.get(), &hitPoint) == 0) {
        Vector3D disp_rem = hitPoint - mol.pos();

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
  double scale = sqrt(4*spec.d()*dt);
  Vector3D disp{scale * state.rng_norm(), scale * state.rng_norm(),
    scale * state.rng_norm()};

  // diffuse and collide until we're at the end of our diffusion step
  while (collide(state, mol, disp)) {}
  if (disp.norm2() > 0) {
    mol.moveTo(mol.pos() + disp);
  }
  assert(mol.pos().x > -0.2 && mol.pos().x < 0.2);
  assert(mol.pos().y > -0.2 && mol.pos().y < 0.2);
  assert(mol.pos().z > -0.2 && mol.pos().z < 0.2);
  return true;
}
