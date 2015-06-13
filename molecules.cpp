// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include "molecules.hpp"


// Mol constructor
Mol::Mol(double t, const MolSpecies* spec) : t_{t}, spec_{spec} {}


// Mol destructor - provide implementation since we use a pure virtual destructor
Mol::~Mol() {};


// VolMol constructor
VolMol::VolMol(double t, const MolSpecies* spec, const Vector3D& pos)
  : Mol(t, spec), pos_{pos} {}


// moveTo moves the volume molecule to a new position
void VolMol::moveTo(const Vector3D& to) {
  pos_ = to;
}
