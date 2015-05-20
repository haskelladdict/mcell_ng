// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <iostream>

#include "molecules.hpp"
#include "rng.hpp"
#include "species.hpp"
#include "state.hpp"

using std::cout;
using std::endl;

int main() {
  cout << "Hello world" << endl;

  MolSpecies A(1e-6, "A");
  vector3D pos{0.0, 0.0, 0.0};
  VolMol a(0.0, &A, pos);

  cout << a.spec()->name() << endl;
  cout << a.pos().x << " " << a.pos().y << " " << a.pos().z << endl;
  vector3D newPos{0.5,0.0,0.5};
  a.moveTo(newPos);
  cout << a.pos().x << " " << a.pos().y << " " << a.pos().z << endl;

  State state;

  RngNorm rng(323);
  for (int i = 0; i < 10; ++i) {
    cout << rng.gen() << endl;
  }
}
