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
  auto aSpecPtr = state.add_mol_species(std::make_unique<MolSpecies>(600, "A"));
  for (int i=0; i < 100; ++i) {
    state.add_vol_mol(std::make_unique<VolMol>(0, aSpecPtr, vector3D{0.0,0.0,0.0}));
  }
  auto bSpecPtr = state.add_mol_species(std::make_unique<MolSpecies>(60, "B"));
  for (int i=0; i < 100; ++i) {
    state.add_vol_mol(std::make_unique<VolMol>(0, bSpecPtr, vector3D{0.0,0.0,0.0}));
  }

  if (!write_cellblender("/Users/markus/programming/cpp/mcell_ng/build/viz_data",
    "test", 0, state.volMolMap())) {
    std::cerr << "failed to write output" << endl;
  }
  // do a few diffusion steps
  for (int i=1; i < 1000; ++i) {
    cout << "iteration:   " << i << endl;
    const VolMolMap& vm = state.volMolMap();
    for (auto& sp : vm) {
      for (auto& m : sp.second) {
        double scale = sqrt(4*m->spec()->d()*dt);
        vector3D disp{scale * state.rng_norm(), scale * state.rng_norm(),
          scale * state.rng_norm()};
        m->moveTo(disp += m->pos());
      }
      if (!write_cellblender("/Users/markus/programming/cpp/mcell_ng/build/viz_data",
        "test", i, state.volMolMap())) {
        std::cerr << "failed to write output" << endl;
      }
    }
  }
}
