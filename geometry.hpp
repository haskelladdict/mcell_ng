// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <memory>

#include "util.hpp"

// Wall describes a single triangle wall on a surface.
// NOTE: The wall normal is determined by a right hand rule using v1, v2, and
// v3 in this order
class Wall {

public:

  Wall(struct vector3D p1, struct vector3D p2, struct vector3D p3);


private:

  struct vector3D v1_;
  struct vector3D v2_;
  struct vector3D v3_;
};

using WallPtr = std::unique_ptr<Wall>;
using Mesh = Vec<WallPtr>;


// helper function for creating a WallPtr from three vertices
WallPtr create_wall(struct vector3D p1, struct vector3D x2, struct vector3D x3);


// GeomObject describes a contiguous (closed or open) surface consisting of
// a collection of walls
class GeomObject {

public:
  GeomObject();

  void addWall(WallPtr w);


private:

  Mesh mesh_;
};



#endif
