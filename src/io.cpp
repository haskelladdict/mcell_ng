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
#include <boost/format.hpp>

#include "io.hpp"
#include "util.hpp"


// write_cellblender writes the molecule info at iter to a file name in
// cellblender format located at path.
Error write_cellblender(State& state, std::string path, std::string name,
  int iter) {

  std::string fileName = boost::str(boost::format("%s/%s.cellbin.%04d.dat") %
    path.c_str() % name.c_str() % iter);
  std::ofstream out(fileName);
  if (out.fail()) {
    return Error{"Failed to open file " + fileName};
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

  return noErr;
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


// mcsf_parse_verts extracts a single mesh vertex from a tokenized line
// of mcsf input. The line format is
// Node-ID  Chrt        X-Coordinate        Y-coordinate        Z-coordinate
static void mcsf_parse_vert(const Rvector<std::string>& items, Rvector<Vec3>& verts) {
  assert(items.size() == 5);
  verts.emplace_back(Vec3{std::stod(items[2])
                         ,std::stod(items[3])
                         ,std::stod(items[4])});
}


// mcsf_parse_smplx extracts a single tetrahedron from a tokenized line
// of mcsf input. The line format is
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
static std::string compute_mesh_key(Rvector<size_t> verts) {
  std::sort(verts.begin(), verts.end());
  std::ostringstream sid;
  for (const auto v : verts) {
    sid << v << ":";
  }
  return sid.str();
}


// mark_tet_neighbors marks the two tetrahdra with ID tet1 and tet2 as being
// neighbors across the triangle with meshID
void mark_tet_neighbors(geom::Tets& tets, size_t tet1ID, size_t tet2ID, size_t meshID) {
  assert(tet1ID < tets.size());
  assert(tet2ID < tets.size());

  geom::Tet& tet1 = tets[tet1ID];
  for (size_t i=0; i < tet1.m.size(); ++i) {
    if (tet1.m[i] == meshID) {
      tet1.t[i] = tet2ID;
      break;
    }
    assert(true);  // should never get here
  }

  geom::Tet& tet2 = tets[tet2ID];
  for (size_t i=0; i < tet2.m.size(); ++i) {
    if (tet2.m[i] == meshID) {
      tet2.t[i] = tet1ID;
      break;
    }
    assert(true);  // should never get here
  }
}

// create_tets creates the final MeshElements and Tets based on the list
// of vertices and tetrahedral connectivities.
// The proper mesh orientation for each of a tetrahedron's 4 consititutive
// triangles is determined according to tetgen's vertex numbering shown in
// http://wias-berlin.de/software/tetgen/fformats.ele.html
static void create_tets(const Rvector<Vec3>& verts, const Rvector<SizeTVec>& tetVerts,
  geom::Mesh& mesh, geom::Tets& tets) {
  assert(mesh.size() == 0);
  assert(tets.size() == 0);

  std::unordered_map<std::string, size_t> triangleMap; // map triangle keys to their index
  std::unordered_map<size_t, SizeTVec> tetMap;  // map a triangle index to parent tets

  // for each tet, extract the consititutive triangles, and either create new
  // MeshElements for them or retrieve the index of existing ones. Tets are then
  // assigned the proper indices and orientations of the triangles.
  size_t meshID = 0;
  size_t tetID = 0;
  for (const auto& v : tetVerts) {
    geom::Tet tet;
    size_t faceID = 0;
    for (const auto& f : geom::tetFaces) {
      Rvector<size_t> triangle{v[f[0]], v[f[1]], v[f[2]]};
      std::string key = compute_mesh_key(triangle);
      if (triangleMap.find(key) == triangleMap.end()) {
        triangleMap[key] = meshID;
        tetMap[meshID].push_back(tetID);
        mesh.emplace_back(geom::MeshElement{verts[triangle[0]]
                                           ,verts[triangle[1]]
                                           ,verts[triangle[2]]});
        tet.m[faceID] = meshID;
        tet.o[faceID] = 1;
        meshID++;
      } else {
        size_t id = triangleMap[key];
        tet.m[faceID] = id;
        tet.o[faceID] = -1;
        tetMap[id].push_back(tetID);
      }
      faceID++;
    }
    tetID++;
    tets.emplace_back(tet);
  }

  // connect shared tets across shared faces
  for (const auto& m : tetMap) {
    assert(m.second.size() == 2 || m.second.size() == 1);
    if (m.second.size() == 2) {
      mark_tet_neighbors(tets, m.second[0], m.second[1], m.first);
    }
  }
}


// parse_mcsf_tet_mesh parses an MCSF file containing a tet mesh and creates
// and returns an internal representation of the mesh.
Error parse_mcsf_tet_mesh(const std::string& fileName, geom::Mesh& mesh, geom::Tets& tets) {

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
  size_t numVerts = 0;
  size_t numSimplx = 0;
  Rvector<Vec3> verts;
  Rvector<SizeTVec> tetVerts;
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
  assert(verts.size() == numVerts);
  assert(tetVerts.size() == numSimplx);
  create_tets(verts, tetVerts, mesh, tets);

  return noErr;
}
