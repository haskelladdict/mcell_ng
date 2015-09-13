// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <unordered_map>

#include "geometry.hpp"
#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "util.hpp"


class State {

public:

  State(uint64_t seed = 0);

  // don't allow copy & move operations
  State(const State& s) = delete;
  State& operator=(const State& s) = delete;
  State(State&& s) = delete;
  State& operator=(State&& s) = delete;

  // member functions
  double rng_norm() const {
    return rng_.gen();
  }

  // mesh related functionality
  void add_mesh(const Mesh& mesh);

  const Mesh& get_mesh() const {
    return mesh_;
  }

  MolSpecies* create_species(MolSpecies spec) {
    species_.emplace_back(std::move(spec));
    return &species_[species_.size()-1];
  }

  SpeciesContainer& species() {
    return species_;
  }

  VolMolMap& volMols() {
    return volMolMap_;
  }

private:

  mutable RngNorm rng_;

  Mesh mesh_;
  SpeciesContainer species_;
  VolMolMap volMolMap_;
};

#endif