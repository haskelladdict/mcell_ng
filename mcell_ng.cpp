// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <iostream>
#include <memory>

#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "state.hpp"

using std::cout;
using std::endl;

int main() {
  cout << "Hello world" << endl;

  State state;
  auto aPtr = state.add_mol_species(std::make_unique<MolSpecies>(1e-6, "A"));
  cout << aPtr << endl;
  cout << state.del_mol_species(aPtr) << endl;
}
