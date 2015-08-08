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
  Mol(long specID, double t);
  virtual ~Mol() = 0;


  double t() const {
    return t_;
  }

  long specID() const {
    return specID_;
  }


private:
  long specID_;  // what species are we
  double t_;     // birthday
};


// VolMol describes volume molecules
class VolMol : public Mol {

public:

  VolMol(long specID, const Vector3D& pos, double t);

  const Vector3D& pos() const {
    return pos_;
  }

  void moveTo(const Vector3D& to);


private:
  Vector3D pos_;
};
using VolMolContainer = Vec<VolMol>;


// VolMolMap holds all molecules in the simulation organized by species id
class VolMolMap {

public:

  using mapped_type = std::unordered_map<long, VolMolContainer>::mapped_type;

  VolMol& create(long specID, const Vector3D& pos, double t = 0.0);
  bool del(VolMol& mol);

  mapped_type& operator[](long ID) {
    return volMolMap_[ID];
  }


private:

  std::unordered_map<long, VolMolContainer> volMolMap_;
};



#endif


