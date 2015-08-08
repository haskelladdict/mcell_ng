// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <string>

#include "util.hpp"


// MolSpecies describes molecule species
class MolSpecies {

public:

  MolSpecies(std::string name, double D);

  double d() const {
    return D_;
  }

  std::string name() const {
    return name_;
  }

private:
  double D_ = 0.0;       // diffusion coefficient
  std::string name_;     // species name
};


// SpeciesContainer keeps track of all defined/active MolSpecies
class SpeciesContainer {

public:

  SpeciesContainer() {};


  long create(std::string name, double D);
  bool erase(long ID);

  long size() const {
    return species_.size();
  }

  const MolSpecies& by_ID(long ID) const;
  const MolSpecies& by_name(const std::string &name) const;

  long id(const MolSpecies& spec) const;


private:

  Vec<MolSpecies> species_;
};





#endif
