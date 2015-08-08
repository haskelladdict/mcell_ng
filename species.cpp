// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>


#include "species.hpp"


// MolSpecies constructor
MolSpecies::MolSpecies(std::string name, double D) : D_{D}, name_{name} {}


// create adds a molecule species to the simulation
long SpeciesContainer::create(std::string name, double D) {
  species_.emplace_back(MolSpecies(name, D));
  return species_.size()-1;
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
const MolSpecies& SpeciesContainer::by_ID(long ID) const {
  assert(ID < species_.size());
  return species_[ID];
}


// del deletes the given species from the simulation
bool SpeciesContainer::erase(long ID) {
  assert (ID < species_.size());
  species_.erase(species_.begin() + ID);
  return true;
}


// id returns the species id for the given molecular species
long SpeciesContainer::id(const MolSpecies& spec) const {
  auto it = std::find_if(species_.begin(), species_.end(), [&spec] (const auto& p) {
    return &p == &spec;
  });
  if (it == species_.end()) {
    throw;
  }
  return std::distance(species_.begin(), it);
}

