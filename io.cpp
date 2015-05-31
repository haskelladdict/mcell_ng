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
bool write_cellblender(std::string path, std::string name, int iter,
  const VolMols& mols) {

  if (mols.size() == 0) {
    return false;
  }

  // sort molecules according to species type
  std::unordered_map<std::string, Vec<const VolMol*>> molMap;
  for (const auto& m : mols) {
    std::string name = m->spec()->name();
    if (molMap.find(name) == molMap.end()) {
      molMap[name] = Vec<const VolMol*>();
    }
    molMap[name].push_back(m.get());
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
  for (const auto &sp : molMap) {
    std::string name = sp.first;
    unsigned char length = name.length();
    out.write(reinterpret_cast<char*>(&length), sizeof(length));
    out.write(name.c_str(), length*sizeof(char));

    unsigned char type = 0;   // 0 indicated volume molecules
    out.write(reinterpret_cast<char*>(&type), sizeof(type));

    unsigned int numMols = 3 * sp.second.size();
    out.write(reinterpret_cast<char*>(&numMols), sizeof(numMols));
    for (const auto& m : sp.second) {
      float mposx = m->pos().x;
      out.write(reinterpret_cast<char*>(&mposx), sizeof(mposx));
      float mposy = m->pos().y;
      out.write(reinterpret_cast<char*>(&mposy), sizeof(mposy));
      float mposz = m->pos().z;
      out.write(reinterpret_cast<char*>(&mposz), sizeof(mposz));
    }
  }
  out.close();

  return true;
}















