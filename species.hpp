// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <memory>
#include <string>

// MolSpecies describes 2D and 3D species
class MolSpecies {

public:
  MolSpecies(double D, std::string name, bool is3D = true);

  double d() const {
    return D_;
  }

  std::string name() const {
    return name_;
  }

  bool is3D() const {
    return is3D_;
  }

private:
  double D_ = 0.0;       // diffusion coefficient
  std::string name_;     // species name
  bool is3D_;
};

using MolSpecPtr = std::unique_ptr<MolSpecies>;


#endif
