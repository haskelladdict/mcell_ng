// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// add_MolSpecies adds a molecule species to the simulation
const MolSpecies* State::create_MolSpecies(std::string name, double D) {
  MolSpecPtr m = std::make_unique<MolSpecies>(name, D);
  MolSpecies* mptr = m.get();
  species_.emplace_back(std::move(m));
  return mptr;
}


// get_MolSpecies returns a pointer to the underlying species of the
// given name
const MolSpecies* State::get_MolSpecies(std::string name) const {
  auto it = std::find_if(species_.begin(), species_.end(), [&](const auto& p) {
    return p->name() == name;
  });
  if (it == species_.end()) {
    nullptr;
  }
  return it->get();
}

// del_MolSpecies deletes the given molecule species from the simulation
// NOTE: This function will also delete all associate molecules from the model
bool State::del_MolSpecies(const MolSpecies* sp) {
  auto it = std::find_if(species_.begin(), species_.end(), [&](const auto& t) {
    return t.get() == sp;
  });
  if (it == species_.end()) {
    return false;
  }

  // remove all molecules of species sp
  if (volMolMap_.find(sp->name()) != volMolMap_.end()) {
    volMolMap_.erase(sp->name());
  }
  species_.erase(it);
  return true;
}


// add_vol_mol adds a new volume molecule to the simulation
// NOTE: We keep the molecules separated by species via an unordered_map
const VolMol* State::create_VolMol(double t, const MolSpecies* spec,
  const vector3D& pos) {
  VolMolPtr m = std::make_unique<VolMol>(t, spec, pos);
  VolMol *vm = m.get();
  auto name = spec->name();
  if (volMolMap_.find(name) == volMolMap_.end()) {
    volMolMap_[name] = VolMols{};
  }
  volMolMap_[name].emplace_back(std::move(m));
  return vm;
}


// del_vol_mol removes the provided volume molecule from the simulation
bool State::del_VolMol(const VolMol* m) {
  VolMols& vm = volMolMap_[m->spec()->name()];
  auto it = std::find_if(vm.begin(), vm.end(), [&](const auto& p) {
    return p.get() == m;
  });
  if (it != vm.end()) {
    vm.erase(it);
    return true;
  }
  return false;
}

