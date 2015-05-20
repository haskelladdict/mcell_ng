// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details


#include "species.hpp"


// MolSpecies constructor
MolSpecies::MolSpecies(double D, std::string name, bool is3D)
  : D_{D}, name_{name}, is3D_{is3D} {}
