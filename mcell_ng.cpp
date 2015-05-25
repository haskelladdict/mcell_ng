// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>
#include <iostream>
#include <memory>

#include "io.hpp"
#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "state.hpp"

using std::cout;
using std::endl;

int main() {
  cout << "Hello world" << endl;

  double dt = 1e-6;

  State state;
  auto aSpecPtr = state.add_mol_species(std::make_unique<MolSpecies>(1e-6, "A"));
  for (int i=0; i < 100; ++i) {
    state.add_vol_mol(std::make_unique<VolMol>(0, aSpecPtr, vector3D{0.0,0.0,0.0}));
  }

  // do one diffusion step
  for (const auto& m : state.volMols()) {
    vector3D disp{sqrt(4*m->spec()->d()*dt) * state.rng_norm(),
      sqrt(4*m->spec()->d()*dt) * state.rng_norm(),
      sqrt(4*m->spec()->d()*dt) * state.rng_norm()
    };
    m->moveTo(disp += m->pos());
  }
  write_cellblender("/Users/markus/programming/cpp/mcell_ng/build/viz_data",
    "test", 1, state.volMols());
}
