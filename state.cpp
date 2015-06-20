// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>

#include "state.hpp"

// constructor
State::State(uint64_t seed) : rng_{seed} {}


// create_Mesh adds a new mesh object to the simulation
Mesh* State::create_Mesh(std::string name) {
  MeshPtr m = std::make_unique<Mesh>(name);
  Mesh* mptr = m.get();
  meshes_.emplace_back(std::move(m));
  return mptr;
}


// create_MolSpecies adds a molecule species to the simulation
const MolSpecies* State::create_MolSpecies(std::string name, double D) {
  MolSpecPtr m = std::make_unique<MolSpecies>(name, D);
  MolSpecies* mptr = m.get();
  species_.emplace_back(std::move(m));
  speciesNames_.emplace_back(name);
  return mptr;
}


// get_MolSpecies returns a pointer to the underlying species of the
// given name
const MolSpecies* State::get_MolSpecies(std::string name) const {
  auto it = std::find_if(species_.begin(), species_.end(), [&](const auto& p) {
    return p->name() == name;
  });
  if (it == species_.end()) {
    return nullptr;
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

  // remove species name
  auto nit = std::find(speciesNames_.begin(), speciesNames_.end(), sp->name());
  assert(nit != speciesNames_.end());
  speciesNames_.erase(nit);

  species_.erase(it);
  return true;
}


// create_VolMol adds a new volume molecule to the simulation
// NOTE: We keep the molecules separated by species via an unordered_map
const VolMol* State::create_VolMol(double t, const MolSpecies* spec,
  const Vector3D& pos) {
  VolMolPtr m = std::make_unique<VolMol>(t, spec, pos);
  VolMol *vm = m.get();
  auto name = spec->name();
  if (volMolMap_.find(name) == volMolMap_.end()) {
    volMolMap_[name] = VolMols{};
  }
  volMolMap_[name].emplace_back(std::move(m));
  return vm;
}


// del_VolMol removes the provided volume molecule from the simulation
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

