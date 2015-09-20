// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef DIFFUSE_HPP
#define DIFFUSE_HPP

#include "state.hpp"


bool diffuse(State& state, const MolSpecies& spec, VolMol& mol, double dt);

bool process_tet(const geom::Tet& test, const geom::Mesh& mesh, TetMols& mols);

#endif
