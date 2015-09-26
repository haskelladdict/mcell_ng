// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include "geometry.hpp"
#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "util.hpp"


class State {

public:

  State(double dt, uint64_t seed = 0);

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
  void add_geometry(const geom::Mesh& mesh, const geom::Tets& tets);

  double dt() const noexcept {
    return dt_;
  }

  const geom::Mesh& mesh() const noexcept {
    return mesh_;
  }

  const geom::Tets& tets() const noexcept {
    return tets_;
  }

  TetMolState& tetMols(size_t i) {
    return tetMolStates_[i];
  }

  size_t create_species(MolSpecies spec) {
    species_.emplace_back(std::move(spec));
    return species_.size() - 1;
  }

  const SpeciesContainer& species() const noexcept {
    return species_;
  }

private:

  mutable RngNorm rng_;

  double dt_;

  geom::Mesh mesh_;
  geom::Tets tets_;
  TetMolStates tetMolStates_;

  SpeciesContainer species_;
};

#endif
