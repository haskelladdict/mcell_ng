// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <memory>
#include <utility>

#include "geometry.hpp"


// helper function for creating a WallPtr from three vertices
Wall::ptr create_wall(struct vector3D p1, struct vector3D p2, struct vector3D p3) {
  return std::make_unique<Wall>(p1, p2, p3);
}


// Wall constructor
Wall::Wall(struct vector3D p1, struct vector3D p2, struct vector3D p3) :
  v1_{p1}, v2_{p2}, v3_{p3} {}


// GeomObject constructor
GeomObject::GeomObject() {};


// addWall adds a single wall the GeomObject's existing mesh
void GeomObject::addWall(Wall::ptr w) {
  mesh_.push_back(std::move(w));
}

