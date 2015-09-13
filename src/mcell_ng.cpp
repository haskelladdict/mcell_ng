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


using std::cerr;
using std::cout;
using std::endl;

int main() {
  cout << "Hello world" << endl;

  double dt = 1e-6;
  const std::string outDir = "/Users/markus/programming/cpp/mcell_ng/build/viz_data";
  State state;

  Mesh m;
  Tets t;
  std::string meshFile = "../mcell_ng_trunk/tests/cube.m";
  auto e = parse_mcsf_tet_mesh(meshFile, m, t);
  if (e.err) {
    cerr << e.desc << endl;
    exit(1);
  }

#if 0
  MeshPropPtr prop1 = std::make_shared<MeshProp>();
  prop1->name = "cube1";
  auto cube1 = create_rectangle(Vec3{-0.1, -0.1, -0.1}, Vec3{0.1, 0.1, 0.1}, prop1);
  state.add_mesh(cube1);

  MeshPropPtr prop2 = std::make_shared<MeshProp>();
  prop2->name = "cube2";
  auto cube2 = create_rectangle(Vec3{-0.01, -0.01, -0.01}, Vec3{0.01, 0.01, 0.01}, prop2);
  state.add_mesh(cube2);

  auto aSpec = state.create_species(MolSpecies("A", 600));
  for (int i=0; i < 10000; ++i) {
    state.volMols().create(aSpec, Vec3{0.0,0.0,0.0});
  }
#endif
/*
  auto bID = state.species().create("B", 900);
  for (int i=0; i < 12000; ++i) {
    state.volMols().create(bID, Vec3{0.05,0.05,0.05});
  }
*/


  if (!write_cellblender(state, outDir, "test", 0)) {
    std::cerr << "failed to write output" << endl;
  }


  // do a few diffusion steps
  for (int i=1; i < 1; ++i) {
    cout << "iteration:   " << i << endl;
    for (auto& spec : state.species()) {
      for (auto& m : state.volMols()[spec.name()]) {
        if (!diffuse(state, spec, m, dt)) {
          cout << "error diffusing molecule " << spec.name() << endl;
        }
      }
    }

    if (i % 10 == 0) {
      if (!write_cellblender(state, outDir, "test", i)) {
        std::cerr << "failed to write output" << endl;
      }
    }
  }
}