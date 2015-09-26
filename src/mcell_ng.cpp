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

  const std::string outDir = "/Users/markus/programming/cpp/mcell_ng/build/viz_data";
  State state(1e-6);

  geom::Mesh mesh;
  geom::Tets tets;
  Error e;
  std::string meshFile = "../mcell_ng_trunk/tests/cube.mcsf";
  //std::string meshFile = "../mcell_ng_trunk/tests/sphere.mcsf";
  std::tie(mesh, tets, e) = parse_mcsf_tet_mesh(meshFile);
  if (e.err) {
    cerr << e.desc << endl;
    exit(1);
  }

  state.add_geometry(mesh, tets);

  auto aSpecID = state.create_species(MolSpecies("A", 600));
  auto& tetMols = state.tetMols(0);
  for (int i=0; i < 10000; ++i) {
    VolMol mol{aSpecID, geom::Vec3{-0.000001,0.0,0.0}, 0.0};
    tetMols.activeMols.add(std::move(mol));
  }
/*
  auto bID = state.species().create("B", 900);
  for (int i=0; i < 12000; ++i) {
    state.volMols().create(bID, Vec3{0.05,0.05,0.05});
  }
*/

  e = write_cellblender(state, outDir, "test", 0);
  if (e.err) {
    cerr << "write_cellblender: " << e.desc << endl;
    exit(1);
  }

  // do a few diffusion steps
  for (int i=1; i < 10; ++i) {
    cout << "iteration:   " << i << endl;

    for (size_t tetID = 0; tetID < state.tets().size(); ++tetID) {
      process_tet(state, tetID);
    }
#if 0
    for (auto& spec : state.species()) {
      for (auto& m : state.volMols()[spec.name()]) {
        if (!diffuse(state, spec, m, dt)) {
          cout << "error diffusing molecule " << spec.name() << endl;
        }
      }
    }
#endif

    if (i % 10 == 0) {
      e = write_cellblender(state, outDir, "test", i);
      if (e.err) {
        cerr << "write_cellblender :" << e.desc << endl;
      }
    }
  }
}
