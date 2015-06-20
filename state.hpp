// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <unordered_map>

#include "geometry.hpp"
#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "util.hpp"


using Meshes = Vec<MeshPtr>;
using Species = Vec<MolSpecPtr>;
using VolMols = Vec<VolMolPtr>;
using VolMolMap = std::unordered_map<std::string, VolMols>;


class State {

public:

  State(uint64_t seed = 0);

  // don't allow copy & move operations
  State(const State& s) = delete;
  State& operator=(const State& s) = delete;
  State(State&& s) = delete;
  State& operator=(State&& s) = delete;

  // member functions
  double rng_norm() const {
    return rng_.gen();
  }

  Mesh* create_Mesh(std::string name);

  const MolSpecies* create_MolSpecies(std::string name, double D);
  const MolSpecies* get_MolSpecies(std::string name) const;
  Vec<std::string> get_MolSpeciesNames() const {
    return speciesNames_;
  }
  bool del_MolSpecies(const MolSpecies* sp);

  const VolMol* create_VolMol(double t, const MolSpecies* spec, const Vector3D& pos);
  const VolMols& get_VolMols(std::string species) const {
    return volMolMap_.at(species);
  }
  bool del_VolMol(const VolMol* m);

private:

  mutable RngNorm rng_;

  Meshes meshes_;
  Species species_;
  VolMolMap volMolMap_;
  Vec<std::string> speciesNames_;
};

#endif
