// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
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

  // get a copy of list of molecules and sort them according to species type
  Vec<const VolMol*> sortedMols(mols.size());
  auto it = sortedMols.begin();
  for (const auto& m : mols) {
    *(it++) = m.get();
  }
  std::sort(sortedMols.begin(), sortedMols.end(),
    [](const auto& p1, const auto& p2) {
      return p1->spec()->name() < p2->spec()->name();
  });

  std::stringstream sts;
  sts << path << "/" << name << "." << "cellbin." << iter << ".dat";
  std::ofstream out(sts.str());
  if (out.fail()) {
    return false;
  }

  uint32_t version = 1;
  out.write(reinterpret_cast<char*>(&version), sizeof(uint32_t));



  return true;
}

