// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

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

    auto& mols = state.volMols()[spec.name()];
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


// mcsf_is_comment checks if a line in an mcsf file is a comment, i.e. starts
// with a %
static bool mcsf_is_comment(const std::string& line) {
  if (!line.empty() && line[0] == '%') {
    return true;
  }
  return false;
}


// mcsf_file_check tests if the first line in the parsed file contains the
// the expected mcsf_begin=1
static bool mcsf_file_check(const std::string& line) {
  if (boost::trim_copy(line) != "mcsf_begin=1;") {
    return false;
  }
  return true;
}


// mcsf_parse_verts extracts a single mesh vertex from a single tokenized line
// of input. The line format is
// Node-ID  Chrt        X-Coordinate        Y-coordinate        Z-coordinate
static void mcsf_parse_vert(const Rvector<std::string>& items, Rvector<Vec3>& verts) {
  assert(items.size() == 5);
  verts.emplace_back(Vec3{std::stod(items[2])
                         ,std::stod(items[3])
                         ,std::stod(items[4])});
}


// mcsf_parse_smplx extracts a single tetrahedron from a single tokenized line
// of input. The line format is
// Simp-ID Grp    Mat          Face-Types                      Vertex-Numbers
static void mcsf_parse_smplx(const Rvector<std::string>& items,
  Rvector<Rvector<size_t>>& tetVerts) {
  assert(items.size() == 11);
  tetVerts.emplace_back(Rvector<size_t>{std::stoul(items[7])
                                       ,std::stoul(items[8])
                                       ,std::stoul(items[9])
                                       ,std::stoul(items[10])});
}


// mcsf_compute_mesh_key computes the unique key for a set of vertex indices
// by sorting them.
// NOTE: Since we sort the vector we take a copy instead of a ref
static std::string mcsf_compute_mesh_key(Rvector<size_t> verts) {
  std::sort(verts.begin(), verts.end());
  std::ostringstream sid;
  for (const auto v : verts) {
    sid << v << ":";
  }
  return sid.str();
}


// mcsf_create_tets creates the final MeshElements and Tets based on the list
// of vertices and tetrahedral connectivities.
// The proper mesh orientation for each of a tetrahedron's 4 consititutive
// triangles is determined according to tetgen's vertex numbering shown in
// http://wias-berlin.de/software/tetgen/fformats.ele.html
static void mcsf_create_tets(const Rvector<Vec3>& verts,
  const Rvector<Rvector<size_t>>& tetVerts, Mesh& mesh, Tets& tets) {
  assert(mesh.size() == 0);
  assert(tets.size() == 0);
  Rvector<Rvector<size_t>> tetFaces{Rvector<size_t>{0, 2, 1}
                                   ,Rvector<size_t>{0, 1, 3}
                                   ,Rvector<size_t>{1, 2, 3}
                                   ,Rvector<size_t>{2, 0, 3}};
  std::unordered_map<std::string, size_t> meshMap;
  size_t meshCount = 0;
  for (const auto& v : tetVerts) {
    Tet tet;
    size_t faceCount = 0;
    for (const auto& f : tetFaces) {
      Rvector<size_t> mvs{v[f[0]], v[f[1]], v[f[2]]};
      std::string key = mcsf_compute_mesh_key(mvs);
      if (meshMap.find(key) == meshMap.end()) {
        meshMap[key] = meshCount;
        mesh.emplace_back(MeshElement{verts[mvs[0]], verts[mvs[1]], verts[mvs[2]]});
        tet.m[faceCount] = meshCount;
        tet.o[faceCount] = 1;
        meshCount++;
      } else {
        tet.m[faceCount] = meshMap[key];
        tet.o[faceCount] = -1;
      }
      faceCount++;
    }
    tets.emplace_back(tet);
  }
}






// parse_mcsf_tet_mesh parses an MCSF file containing a tet mesh and creates
// and returns an internal representation of the mesh.
Error parse_mcsf_tet_mesh(const std::string& fileName, Mesh& mesh, Tets& tets) {

  std::ifstream file(fileName);
  if (!file.is_open()) {
    return Error{"failed to open file " + fileName};
  }

  std::string line;
  if (!getline(file, line) || !mcsf_file_check(line)) {
    return Error{fileName + "is not an mcsf mesh file"};
  }

  bool inVerts = false;
  bool inSmplx = false;
  long numVerts = 0;
  long numSimplx = 0;
  Rvector<Vec3> verts;
  Rvector<Rvector<size_t>> tetVerts;
  // need a try block since string to integer/double conversion might throw
  try {
    while(getline(file, line)) {
      boost::trim(line);
      if (line.length() == 0 || mcsf_is_comment(line)) {
        continue;
      }
      Rvector<std::string> items;
      boost::split(items, line, boost::is_any_of("\t "), boost::token_compress_on);
      if (items[0] == "vertices") {
        assert(items.size() == 3);
        numVerts = std::stoll(items[2]);
      } else if (items[0] == "simplices") {
        assert(items.size() == 3);
        numSimplx = std::stoll(items[2]);
      } else if (items[0] == "vert=[") {
        assert(inSmplx == false);
        inVerts = true;
      } else if (items[0] == "simp=[") {
        assert(inVerts == false);
        inSmplx = true;
      } else if (items[0] == "];" && inSmplx) {
        inSmplx = false;
      } else if (items[0] == "];" && inVerts) {
        inVerts = false;
      } else if (inVerts) {
        mcsf_parse_vert(items, verts);
      } else if (inSmplx) {
        mcsf_parse_smplx(items, tetVerts);
      }
    }
  } catch (std::invalid_argument& e) {
    return Error("could not parse mcsf file");
  }
  mcsf_create_tets(verts, tetVerts, mesh, tets);
//  std::cout << numVerts << " :: " << numSimplx << std::endl;
//  std::cout.precision(10);
//  std::cout << verts << std::endl;
//  std::cout << tetVerts << std::endl;

  return noErr;
}
