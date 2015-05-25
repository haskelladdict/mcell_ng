// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// add_mol_species adds a molecule species to the simulation
const MolSpecies* State::add_mol_species(MolSpecPtr m) {
  const MolSpecies *sp = m.get();
  species_.push_back(std::move(m));
  return sp;
}


// del_mol_species deletes the given molecule species from the simulation
bool State::del_mol_species(const MolSpecies* m) {
  auto it = std::find_if(species_.begin(), species_.end(), [&](const auto& p) {
    return p.get() == m;
  });
  if (it != species_.end()) {
    species_.erase(it);
    return true;
  }
  return false;
}


// add_vol_mol adds a new volume molecule to the simulation
const VolMol* State::add_vol_mol(VolMolPtr m) {
  const VolMol *vm = m.get();
  volMols_.push_back(std::move(m));
  return vm;
}


// del_vol_mol removes the provided volume molecule from the simulation
bool State::del_vol_mol(const VolMol* m) {
  auto it = std::find_if(volMols_.begin(), volMols_.end(), [&](const auto& p) {
    return p.get() == m;
  });
  if (it != volMols_.end()) {
    volMols_.erase(it);
    return true;
  }
  return false;
}










