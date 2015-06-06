// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <memory>

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

  Mesh();

  void add_vertex(double x, double y, double z);
  void add_triangle(int v1, int v2, int v3);


private:

  Vec<vector3D> verts_;
  Vec<Triangle> triangles_;
};



#endif
