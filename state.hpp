// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include <vector>

#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"

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
  bool del_mol_species(const MolSpecies *m);


private:

  RngNorm rng_;

  std::vector<MolSpecPtr> species_;
  std::vector<VolMolPtr> volMols_;
};

#endif
