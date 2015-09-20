// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef MOLECULES_HPP
#define MOLECULES_HPP

#include <memory>
#include <unordered_map>

#include "species.hpp"
#include "vector.hpp"

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
  double t_;                // birthday
};


// VolMol describes volume molecules
class VolMol : public Mol {

public:

  VolMol(const MolSpecies* spec, const geom::Vec3& pos, double t);

  const geom::Vec3& pos() const noexcept {
    return pos_;
  }

  void moveTo(const geom::Vec3& to);


private:
  geom::Vec3 pos_;
};


// MolContainer is a simple overload of Rvector for efficient removal of
// elements. Instead of removing elements from within the vector MolContainer
// swaps the removed elements with the last element in the vector and then
// removes the last element.
template<typename T>
class MolContainer : public Rvector<T> {

public:
  using Rvector<T>::Rvector;
  using iter = typename Rvector<T>::iterator;

  iter erase(iter pos) {
    auto last = Rvector<T>::end() - 1;
    if (pos != last) {
      std::swap(*pos, *last);
    }
    return Rvector<T>::erase(last);
  }
};

using VolMolContainer = MolContainer<VolMol>;


// VolMolMap holds all molecules in the simulation organized by species name
class VolMolMap {

public:
  using mapped_type = std::unordered_map<std::string, VolMolContainer>::mapped_type;

  // add an existing VolMol and take possession
  void add(VolMol&& volMol);

  // delete a volume molecule
  bool del(VolMol& mol);

  mapped_type& operator[](std::string specName) {
    return volMolMap_[specName];
  }

private:

  std::unordered_map<std::string, VolMolContainer> volMolMap_;
};


// MolState keeps track of all molecules within a tet
struct MolState {

  // active molecules located in this tet
  VolMolMap activeMols;

  // molecules enterting this tet (from neighboring tets)
  VolMolContainer inMols;

  // molecules leaving this tet for one of the four neighbors
  Rvector<VolMolContainer> outMols{4};
};

using TetMols = Rvector<MolState>;


#endif
