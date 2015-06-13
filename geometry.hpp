// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <memory>
#include <ostream>
#include <string>

#include "util.hpp"



// triangle describes a single triangle on a mesh. It consists of the indexes
// of the vertices making up the triangle.
struct Triangle {
  int v1;
  int v2;
  int v3;
};


// Mesh describes a contiguous (closed or open) surface consisting of
// a collection of vertices and triangular connections between them.
class Mesh {

public:

  Mesh(std::string name);

  friend std::ostream& operator<<(std::ostream& os, const Mesh& m);

  void add_vertex(Vector3D&& x);
  void add_triangle(int v1, int v2, int v3);


private:

  std::string name_;
  Vec<Vector3D> verts_;
  Vec<Triangle> triangles_;
};
using MeshPtr = std::unique_ptr<Mesh>;


// provide operator<< for Mesh for debugging purposes
std::ostream& operator<<(std::ostream& os, const Mesh& m);


// helper function for creating a rectangular geometry primitive
void create_rectangle(Mesh* mesh, const Vector3D& llc, const Vector3D& urc);


using triangleID = std::vector<Triangle>::size_type;

// Region describes a collection of triangles on a Mesh
class Region {

public:

  Region(std::string name, const Mesh* mesh);

  void add_element(triangleID id);
  void add_elements(const Vec<triangleID>& ids);


private:

  std::string name_;
  const Mesh* parent_mesh_;
  Vec<triangleID> triangles_;
};



#endif




