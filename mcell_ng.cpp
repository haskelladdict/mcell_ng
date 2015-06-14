// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>
#include <iostream>
#include <memory>

#include "geometry.hpp"
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

  auto meshPtr = state.create_Mesh("cube");
  auto meshElems = create_rectangle(meshPtr, Vector3D{0.0, 0.0, 0.0},
    Vector3D{1.0, 1.0, 1.0});
  for (auto& m : meshElems) {
    //m->add_meshElementProperty(nullptr);
  }
  cout << *meshPtr << endl;


  auto aSpecPtr = state.create_MolSpecies("A", 600);
  for (int i=0; i < 1000; ++i) {
    state.create_VolMol(0, aSpecPtr, Vector3D{0.0,0.0,0.0});
  }

  auto bSpecPtr = state.create_MolSpecies("B", 60);
  for (int i=0; i < 100; ++i) {
    state.create_VolMol(0, bSpecPtr, Vector3D{0.0,0.0,0.0});
  }

  //cout << state.del_MolSpecies(aSpecPtr) << endl;
  auto p = state.get_MolSpecies("A");
  if (p != nullptr) {
    cout << p->name() << endl;
  } else {
    cout << "molecule species not present" << endl;
  }

  if (!write_cellblender("/Users/markus/programming/cpp/mcell_ng/build/viz_data",
    "test", 0, state.volMolMap())) {
    std::cerr << "failed to write output" << endl;
  }
  // do a few diffusion steps
  for (int i=1; i < 2; ++i) {
    cout << "iteration:   " << i << endl;
    const VolMolMap& vm = state.volMolMap();
    for (auto& sp : vm) {
      cout << sp.first << endl;
      for (auto& m : sp.second) {
        double scale = sqrt(4*m->spec()->d()*dt);
        Vector3D disp{scale * state.rng_norm(), scale * state.rng_norm(),
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
