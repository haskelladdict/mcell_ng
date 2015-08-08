// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <string>

#include "util.hpp"


// MolSpecies describes molecule species
class MolSpecies {

public:

  MolSpecies(std::string name, long id, double d);

  double D() const {
    return d_;
  }

  std::string name() const {
    return name_;
  }

  long ID() const {
    return id_;
  }


private:
  double d_ = 0.0;       // diffusion coefficient
  long id_;              // species id
  std::string name_;     // species name
};


// SpeciesContainer keeps track of all defined/active MolSpecies
class SpeciesContainer {

public:

  using iterator = Vec<MolSpecies>::iterator;
  using const_iterator = Vec<MolSpecies>::const_iterator;

  SpeciesContainer() {};

  long create(std::string name, double D);
  bool erase(long ID);

  long size() const {
    return species_.size();
  }

  iterator begin() {
    return species_.begin();
  }

  const_iterator begin() const {
    return species_.begin();
  }

  iterator end() {
    return species_.end();
  }

  const_iterator end() const {
    return species_.end();
  }

  const MolSpecies& by_ID(long ID) const;
  const MolSpecies& by_name(const std::string &name) const;


private:

  Vec<MolSpecies> species_;
};





#endif
