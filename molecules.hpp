// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef MOLECULES_HPP
#define MOLECULES_HPP

#include <memory>
#include <unordered_map>

#include "species.hpp"
#include "util.hpp"

// Mol is the abstract base class for describing 2D and 3D molecules
class Mol {

public:
  Mol(const MolSpecies* spec, double t);
  virtual ~Mol() = 0;


  double t() const {
    return t_;
  }

  const MolSpecies* spec() const {
    return spec_;
  }


private:
  const MolSpecies* spec_;  // what species are we
  double t_;     // birthday
};


// VolMol describes volume molecules
class VolMol : public Mol {

public:

  VolMol(const MolSpecies* spec, const Vec3& pos, double t);

  const Vec3& pos() const {
    return pos_;
  }

  void moveTo(const Vec3& to);


private:
  Vec3 pos_;
};
using VolMolContainer = Vec<VolMol>;


// VolMolMap holds all molecules in the simulation organized by species name
class VolMolMap {

public:

  using mapped_type = std::unordered_map<std::string, VolMolContainer>::mapped_type;

  VolMol& create(const MolSpecies *spec, const Vec3& pos, double t = 0.0);
  bool del(VolMol& mol);

  mapped_type& operator[](std::string specName) {
    return volMolMap_[specName];
  }


private:

  std::unordered_map<std::string, VolMolContainer> volMolMap_;
};



#endif


