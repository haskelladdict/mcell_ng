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
  Mol(double t, long specID);
  virtual ~Mol() = 0;


  double t() const {
    return t_;
  }

  long specID() const {
    return specID_;
  }


private:
  double t_;     // birthday
  long specID_;  // what species are we
};


// VolMol describes volume molecules
class VolMol : public Mol {

public:

  VolMol(double t, long specID, const Vector3D& pos);

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

  VolMol& create(long specID, double t, const Vector3D& pos);
  bool del(VolMol& mol);

  mapped_type& operator[](long ID) {
    return volMolMap_[ID];
  }


private:

  std::unordered_map<long, VolMolContainer> volMolMap_;
};



#endif


