// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <memory>
#include <string>

// VolMolSpecies describes volume molecules
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

using MolSpecPtr = std::unique_ptr<MolSpecies>;

#endif
