// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details


#include <algorithm>

#include "molecules.hpp"


// Mol constructor
Mol::Mol(long specID, double t) : specID_{specID}, t_{t} {}


// Mol destructor - provide implementation since we use a pure virtual destructor
Mol::~Mol() {};


// VolMol constructor
VolMol::VolMol(long specID, const Vector3D& pos, double t)
  : Mol(specID, t), pos_{pos} {}


// moveTo moves the volume molecule to a new position
void VolMol::moveTo(const Vector3D& to) {
  pos_ = to;
}


// create adds a new volume molecule to the VolMolMap
VolMol& VolMolMap::create(long specID, const Vector3D& pos, double t) {
  VolMol vm(specID, pos, t);
  if (volMolMap_.find(specID) == volMolMap_.end()) {
    volMolMap_[specID] = VolMolContainer{};
  }
  volMolMap_[specID].emplace_back(std::move(vm));
  size_t index = volMolMap_[specID].size() - 1;
  return volMolMap_[specID][index];
}


// del erases the provided volume molecule from the simulation
bool VolMolMap::del(VolMol& mol) {
  VolMolContainer& vm = volMolMap_[mol.specID()];
  auto it = std::find_if(vm.begin(), vm.end(), [&mol](const auto& p) {
    return &p == &mol;
  });
  if (it != vm.end()) {
    vm.erase(it);
    return true;
  }
  return false;
}
