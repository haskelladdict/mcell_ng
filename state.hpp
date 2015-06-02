// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <unordered_map>

#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "util.hpp"


using Species = Vec<MolSpecies::ptr>;
using VolMols = Vec<VolMol::ptr>;
using VolMolMap = std::unordered_map<const MolSpecies*, VolMols>;


class State {

public:

  State(uint64_t seed = 0);

  // don't allow copy & move operations
  State(const State& s) = delete;
  State& operator=(const State& s) = delete;
  State(State&& s) = delete;
  State& operator=(State&& s) = delete;

  // member functions
  double rng_norm() {
    return rng_.gen();
  }

  const MolSpecies* add_mol_species(MolSpecies::ptr m);
  bool del_mol_species(const MolSpecies* m);

  const VolMol* add_vol_mol(VolMol::ptr m);
  bool del_vol_mol(const VolMol* m);

  const VolMolMap& volMolMap() {
    return volMolMap_;
  }

private:

  RngNorm rng_;

  Species species_;
  VolMolMap volMolMap_;
};

#endif
