// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <string>

#include "util.hpp"


// MolSpecies describes molecule species
class MolSpecies {

public:

  MolSpecies(std::string name, double d) : d_{d}, name_{name} {};

  double D() const {
    return d_;
  }

  std::string name() const {
    return name_;
  }

private:
  double d_ = 0.0;       // diffusion coefficient
  std::string name_;     // species name
};

using SpeciesContainer = Vec<MolSpecies>;


#endif
