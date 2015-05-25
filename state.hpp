// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>

#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "util.hpp"


using Species = Vec<MolSpecPtr>;
using VolMols = Vec<VolMolPtr>;


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

  const MolSpecies* add_mol_species(MolSpecPtr m);
  bool del_mol_species(const MolSpecies* m);

  const VolMol* add_vol_mol(VolMolPtr m);
  bool del_vol_mol(const VolMol* m);

  const VolMols& volMols() {
    return volMols_;
  }


private:

  RngNorm rng_;

  Species species_;
  VolMols volMols_;
};

#endif
