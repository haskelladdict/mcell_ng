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


using Meshes = Vec<geometry::MeshPtr>;
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
  double rng_norm() {
    return rng_.gen();
  }

  //const Mesh* create_mesh()

  const MolSpecies* create_MolSpecies(std::string name, double D);
  const MolSpecies* get_MolSpecies(std::string name) const;
  bool del_MolSpecies(const MolSpecies* sp);

  const VolMol* create_VolMol(double t, const MolSpecies* spec, const vector3D& pos);
  bool del_VolMol(const VolMol* m);

  const VolMolMap& volMolMap() {
    return volMolMap_;
  }

private:

  RngNorm rng_;

  Meshes meshes_;
  Species species_;
  VolMolMap volMolMap_;
};

#endif
