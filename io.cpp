// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>


#include "io.hpp"
#include "util.hpp"


// write_cellblender writes the molecule info at iter to a file name in
// cellblender format located at path.
bool write_cellblender(State& state, std::string path, std::string name,
  int iter) {

  if (state.species().size() == 0) {
    return false;
  }

  char fileName[256];
  if (snprintf(fileName, 255, "%s/%s.cellbin.%04d.dat", path.c_str(),
    name.c_str(), iter) < 0) {
    return false;
  }
  std::ofstream out(fileName);
  if (out.fail()) {
    return false;
  }

  // write version info
  unsigned int version = 1;
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));

  // write molecule info
  for (auto& spec : state.species()) {
    unsigned char length = spec.name().length();
    out.write(reinterpret_cast<char*>(&length), sizeof(length));
    out.write(spec.name().c_str(), length*sizeof(char));

    unsigned char type = 0;   // 0 indicates volume molecules
    out.write(reinterpret_cast<char*>(&type), sizeof(type));

    long id = state.species().spec_to_ID(spec);
    auto& mols = state.volMols().by_ID(id);
    unsigned int numMols = 3 * mols.size();
    out.write(reinterpret_cast<char*>(&numMols), sizeof(numMols));
    for (const auto& m : mols) {
      float mposx = m.pos().x;
      out.write(reinterpret_cast<char*>(&mposx), sizeof(mposx));
      float mposy = m.pos().y;
      out.write(reinterpret_cast<char*>(&mposy), sizeof(mposy));
      float mposz = m.pos().z;
      out.write(reinterpret_cast<char*>(&mposz), sizeof(mposz));
    }
  }
  out.close();

  return true;
}















