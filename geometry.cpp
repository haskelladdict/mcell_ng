// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <iostream>
#include <cassert>
#include <memory>
#include <utility>

#include "geometry.hpp"


// GeomObject constructor
Mesh::Mesh(std::string name) : name_{name} {};


// addVertex adds a new vertex at the given position to the Mesh
void Mesh::add_vertex(Vector3D&& x) {
  verts_.emplace_back(std::move(x));
}


// add_triangle adds a new triangle among the available vertices of a mesh
void Mesh::add_triangle(int v1, int v2, int v3) {
  assert(v1 >= 0 && v1 < verts_.size());
  assert(v2 >= 0 && v2 < verts_.size());
  assert(v3 >= 0 && v3 < verts_.size());

  triangles_.emplace_back(Triangle{v1, v2, v3});
}


// provide operator<< for Mesh for debugging purposes
std::ostream& operator<<(std::ostream& os, const Mesh& m) {
  os << m.verts_.size() << " Vertices:\n";
  for (const auto& v : m.verts_) {
    os << v << "\n";
  }

  os << "\n" << m.triangles_.size() << " Triangles:\n";
  for (const auto& t : m.triangles_) {
    os << "{" << t.v1 << "," << t.v2 << "," << t.v3 << "}\n";
  }
  return os;
}


// helper function for creating a rectangular geometry primitive
void create_rectangle(Mesh* mesh, const Vector3D& llc, const Vector3D& urc) {

  // for rectangle to be well formed llc needs to be smaller than urc for x, y and z
  assert(llc.x < urc.x && llc.y < urc.y && llc.z < urc.z);

  Vector3D diag = urc - llc;
  mesh->add_vertex(Vector3D{llc});
  mesh->add_vertex(Vector3D{llc} + Vector3D{diag.x, 0.0, 0.0});
  mesh->add_vertex(Vector3D{llc} + Vector3D{0.0, diag.y, 0.0});
  mesh->add_vertex(Vector3D{llc} + Vector3D{0.0, 0.0, diag.z});
  mesh->add_vertex(Vector3D{llc} + Vector3D{diag.x, diag.y, 0.0});
  mesh->add_vertex(Vector3D{llc} + Vector3D{diag.x, 0.0, diag.z});
  mesh->add_vertex(Vector3D{llc} + Vector3D{0.0, diag.y, diag.z});
  mesh->add_vertex(Vector3D{urc});

  mesh->add_triangle(0,1,5);
  mesh->add_triangle(0,5,3);
  mesh->add_triangle(1,4,7);
  mesh->add_triangle(1,7,5);
  mesh->add_triangle(4,2,6);
  mesh->add_triangle(4,6,7);
  mesh->add_triangle(2,0,3);
  mesh->add_triangle(2,3,6);
  mesh->add_triangle(5,7,6);
  mesh->add_triangle(5,6,3);
  mesh->add_triangle(0,2,1);
  mesh->add_triangle(1,2,4);
}










