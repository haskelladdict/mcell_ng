// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>

#include "diffuse.hpp"
#include "util.hpp"


bool diffuse(const State& state, VolMol* m, double dt) {
  double scale = sqrt(4*m->spec()->d()*dt);
  Vector3D disp{scale * state.rng_norm(), scale * state.rng_norm(),
    scale * state.rng_norm()};
  m->moveTo(disp += m->pos());

  return true;
}
