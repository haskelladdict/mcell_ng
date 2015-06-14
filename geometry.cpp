// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <utility>

#include "geometry.hpp"


// GeomObject constructor
Mesh::Mesh(std::string name) : name_{name} {};


// addVertex adds a new vertex at the given position to the Mesh
void Mesh::add_vertex(Vector3D&& x) {
  verts_.emplace_back(std::move(x));
}


// add_meshElement adds a new triangle among the available vertices of a mesh
MeshElement* Mesh::add_meshElement(uint64_t v1, uint64_t v2, uint64_t v3,
  const MeshElementProperty* props) {

  assert(v1 < verts_.size());
  assert(v2 < verts_.size());
  assert(v3 < verts_.size());

  meshElems_.emplace_back(std::make_unique<MeshElement>(MeshElement{v1, v2, v3,
    *this, props}));
  return meshElems_.back().get();
}


// provide operator<< for Mesh for debugging purposes
std::ostream& operator<<(std::ostream& os, const Mesh& m) {
  os << m.verts_.size() << " Vertices:\n";
  for (const auto& v : m.verts_) {
    os << v << "\n";
  }

  os << "\n" << m.meshElems_.size() << " Triangles:\n";
  for (const auto& t : m.meshElems_) {
    os << "{" << t->v1() << "," << t->v2() << "," << t->v3() << "}\n";
  }
  return os;
}


// MeshElement constructor
MeshElement::MeshElement(uint64_t v1, uint64_t v2, uint64_t v3,
  const Mesh& parent, const MeshElementProperty* props) : vert1_{v1}, vert2_{v2},
    vert3_{v3}, parent_{parent} {

  if (props != nullptr) {
    props_.emplace_back(props);
  }
}


// add_meshElementProperty adds a new mesh element property to this mesh element
void MeshElement::add_meshElementProperty(const MeshElementProperty* prop) {
  assert(prop != nullptr);
  props_.emplace_back(prop);
}


// delete_meshElementProperty removes an existing mesh element property from
// this mesh element. If the removed property did not exist on the mesh element
// the function returns false and true otherwise.
bool MeshElement::delete_meshElementProperty(const MeshElementProperty* prop) {
  assert(prop != nullptr);
  auto it = std::find(props_.begin(), props_.end(), prop);
  if (it == props_.end()) {
    return false;
  }
  props_.erase(it);
  return true;
}


// helper function for creating a rectangular geometry primitive
Vec<MeshElement*> create_rectangle(Mesh* mesh, const Vector3D& llc,
  const Vector3D& urc) {

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

  Vec<MeshElement*> elems;
  elems.emplace_back(mesh->add_meshElement(0,1,5));
  elems.emplace_back(mesh->add_meshElement(0,5,3));
  elems.emplace_back(mesh->add_meshElement(1,4,7));
  elems.emplace_back(mesh->add_meshElement(1,7,5));
  elems.emplace_back(mesh->add_meshElement(4,2,6));
  elems.emplace_back(mesh->add_meshElement(4,6,7));
  elems.emplace_back(mesh->add_meshElement(2,0,3));
  elems.emplace_back(mesh->add_meshElement(2,3,6));
  elems.emplace_back(mesh->add_meshElement(5,7,6));
  elems.emplace_back(mesh->add_meshElement(5,6,3));
  elems.emplace_back(mesh->add_meshElement(0,2,1));
  elems.emplace_back(mesh->add_meshElement(1,2,4));
  return elems;
}


