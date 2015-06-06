// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef MOLECULES_HPP
#define MOLECULES_HPP

#include <memory>

#include "species.hpp"
#include "util.hpp"

// Mol is the abstract base class for describing 2D and 3D molecules
class Mol {

public:
  Mol(double t, const MolSpecies& spec);
  virtual ~Mol() = 0;

  double t() const {
    return t_;
  }

  const MolSpecies& spec() const {
    return spec_;
  }


private:
  double t_;               // birthday
  const MolSpecies& spec_; // what species are we
};


// VolMol describes volume molecules
class VolMol : public Mol {

public:

  VolMol(double t, const MolSpecies& spec, const vector3D& pos);

  const vector3D& pos() const {
    return pos_;
  }

  void moveTo(const vector3D& to);


private:
  vector3D pos_;
};
using VolMolPtr = std::unique_ptr<VolMol>;

#endif


