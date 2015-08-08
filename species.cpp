// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>


#include "species.hpp"


// MolSpecies constructor
MolSpecies::MolSpecies(std::string name, long id, double d)
  : d_{d}, id_(id), name_{name} {}


// create adds a molecule species to the simulation
long SpeciesContainer::create(std::string name, double d) {
  long speciesID = species_.size();
  species_.emplace_back(MolSpecies(name, speciesID, d));
  return speciesID;
}


// by_name returns a const ref to the species instance corresponding to name
const MolSpecies& SpeciesContainer::by_name(const std::string& name) const {
  auto it = std::find_if(species_.begin(), species_.end(), [&name](const auto& p) {
    return p.name() == name;
  });
  if (it == species_.end()) {
    throw;
  }
  return *it;
}


// by_ID returns a const ref to the species instance corresponding to ID
const MolSpecies& SpeciesContainer::by_ID(long id) const {
  assert(id < static_cast<long>(species_.size()));
  return species_[id];
}


// del deletes the given species from the simulation
bool SpeciesContainer::erase(long id) {
  assert (id < static_cast<long>(species_.size()));
  species_.erase(species_.begin() + id);
  return true;
}
