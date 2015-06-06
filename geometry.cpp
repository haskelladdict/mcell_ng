// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <memory>
#include <utility>

#include "geometry.hpp"

// GeomObject constructor
Mesh::Mesh() {};


// addVertex adds a new vertex to the Mesh at position (x,y,z)
void Mesh::add_vertex(double x, double y, double z) {
  verts_.emplace_back(vector3D{x, y, z});
}


// add_triangle adds a new triangle among the available vertices of a mesh
void Mesh::add_triangle(int v1, int v2, int v3) {
  assert(v1 >= 0 && v1 < verts_.size());
  assert(v2 >= 0 && v2 < verts_.size());
  assert(v3 >= 0 && v3 < verts_.size());

  triangles_.emplace_back(Triangle{v1, v2, v3});
}

