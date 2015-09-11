// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details


#include <algorithm>

#include "molecules.hpp"


// Mol constructor
Mol::Mol(const MolSpecies *spec, double t) : spec_{spec}, t_{t} {}


// Mol destructor - provide implementation since we use a pure virtual destructor
Mol::~Mol() {};


// VolMol constructor
VolMol::VolMol(const MolSpecies *spec, const Vec3& pos, double t)
  : Mol(spec, t), pos_{pos} {}


// moveTo moves the volume molecule to a new position
void VolMol::moveTo(const Vec3& to) {
  pos_ = to;
}


// create adds a new volume molecule to the VolMolMap
VolMol& VolMolMap::create(const MolSpecies *spec, const Vec3& pos, double t) {
  VolMol vm(spec, pos, t);
  auto specName = spec->name();
  if (volMolMap_.find(specName) == volMolMap_.end()) {
    volMolMap_[specName] = VolMolContainer{};
  }
  volMolMap_[specName].emplace_back(std::move(vm));
  size_t index = volMolMap_[specName].size() - 1;
  return volMolMap_[specName][index];
}


// del erases the provided volume molecule from the simulation
bool VolMolMap::del(VolMol& mol) {
  VolMolContainer& vm = volMolMap_[mol.spec()->name()];
  auto it = std::find_if(vm.begin(), vm.end(), [&mol](const auto& p) {
    return &p == &mol;
  });
  if (it != vm.end()) {
    vm.erase(it);
    return true;
  }
  return false;
}
