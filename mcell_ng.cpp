// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cmath>
#include <iostream>
#include <memory>

#include "diffuse.hpp"
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
  auto meshElems = create_rectangle(meshPtr, Vector3D{-0.1, -0.1, -0.1},
    Vector3D{0.1, 0.1, 0.1});
#if 0
  for (auto& m : meshElems) {
    m->add_meshElementProperty(nullptr);
  }
#endif
  cout << *meshPtr << endl;

  auto aID = state.species().create("A", 600);
  for (int i=0; i < 10000; ++i) {
    state.volMols().create(aID, 0, Vector3D{0.0,0.0,0.0});
  }
/*
  auto bSpecPtr = state.create_MolSpecies("B", 60);
  for (int i=0; i < 100; ++i) {
    state.create_VolMol(0, bSpecPtr, Vector3D{0.0,0.0,0.0});
  }
*/
  //cout << state.del_MolSpecies(aSpecPtr) << endl;
  auto p = state.species().by_name("A");
  //if (p != nullptr) {
  cout << p.name() << endl;
  //} else {
  //  cout << "molecule species not present" << endl;
  //}

  if (!write_cellblender(state,
    "/Users/markus/programming/cpp/mcell_ng/build/viz_data", "test", 0)) {
    std::cerr << "failed to write output" << endl;
  }

  // do a few diffusion steps
  for (int i=1; i < 1000; ++i) {
    cout << "iteration:   " << i << endl;
    auto species = state.species();
    for (long s=0; s < species.size(); ++s) {
      auto& spec = species.by_ID(s);
      cout << spec.name() << endl;
      for (auto& m : state.volMols().by_ID(s)) {
        if (!diffuse(state, spec, m, dt)) {
          cout << "error diffusing molecule " << spec.name() << endl;
        }
      }
    }
    if (i % 100 == 0) {
      if (!write_cellblender(state,
        "/Users/markus/programming/cpp/mcell_ng/build/viz_data", "test", i)) {
        std::cerr << "failed to write output" << endl;
      }
    }
  }
}
