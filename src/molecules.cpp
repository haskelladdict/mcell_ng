// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details


#include <algorithm>

#include "molecules.hpp"


// Mol constructor
Mol::Mol(size_t specID, double t) : specID_{specID}, t_{t} {}


// Mol destructor - provide implementation since we use a pure virtual destructor
Mol::~Mol() {};


// VolMol constructor
VolMol::VolMol(size_t specID, const geom::Vec3& pos, double t)
  : Mol(specID, t), pos_{pos} {}


// moveTo moves the volume molecule to a new position
void VolMol::moveTo(const geom::Vec3& to) {
  pos_ = to;
}


// add an existing VolMol and takes possession
void VolMolMap::add(VolMolPtr mol) {
  auto specID = mol->specID();
  if (volMolMap_.find(specID) == volMolMap_.end()) {
    volMolMap_[specID] = VolMolContainer{};
  }
  volMolMap_[specID].emplace_back(std::move(mol));
}


// del erases the provided volume molecule from the simulation
bool VolMolMap::del(VolMolPtr mol) {
  VolMolContainer& vm = volMolMap_[mol->specID()];
  auto it = std::find_if(vm.begin(), vm.end(), [&mol](const auto& p) {
    return p == mol;
  });
  if (it != vm.end()) {
    vm.erase(it);
    return true;
  }
  return false;
}
