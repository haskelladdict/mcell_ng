// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// create_Mesh adds a new mesh object to the simulation
Mesh* State::create_Mesh(std::string name) {
  MeshPtr m = std::make_unique<Mesh>(name);
  Mesh* mptr = m.get();
  meshes_.emplace_back(std::move(m));
  return mptr;
}
