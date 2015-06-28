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
// by providing the respective vertex indices
MeshElement* Mesh::add_meshElement(uint64_t i0, uint64_t i1, uint64_t i2,
  const MeshElementProperty* props) {

  assert(i0 < verts_.size());
  assert(i1 < verts_.size());
  assert(i2 < verts_.size());

  meshElems_.emplace_back(std::make_unique<MeshElement>(MeshElement{i0, i1, i2,
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
    os << "{" << t->i0() << "," << t->i1() << "," << t->i2() << "}\n";
  }
  return os;
}


// MeshElement constructor
MeshElement::MeshElement(uint64_t i0, uint64_t i1, uint64_t i2, const Mesh& parent,
  const MeshElementProperty* props) : vert0_{i0}, vert1_{i1}, vert2_{i2},
  mesh_{parent} {

  u_ = mesh_.get_vertex(i1) - mesh_.get_vertex(i0);
  v_ = mesh_.get_vertex(i2) - mesh_.get_vertex(i0);
  normal_ = cross(u_, v_);
  normal_norm_ = normalize(normal_);

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


// v0, v1, v2 return the locations of vertex 0 through 2 as Vector3Ds
const Vector3D& MeshElement::v0() const {
  return mesh_.get_vertex(vert0_);
}

const Vector3D& MeshElement::v1() const {
  return mesh_.get_vertex(vert1_);
}

const Vector3D& MeshElement::v2() const {
  return mesh_.get_vertex(vert2_);
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


// intersect tests for ray triangle intersections. Possible return values are
//  0: triangle and ray segment intersect, in this case hitPoint contains the
//     location of the intersection point
//  1: triangle and ray intersect but ray segment does not reach the triangle
//  2: triangle and ray do not intersect
//  3: ray and triangle are co-planar
//  4: triangle is degenerate
//
// NOTE: This function was adapted from Dan Sunday
// <http://geomalgorithms.com/a06-_intersect-2.html#intersect3D_RayTriangle()>
int intersect(const Vector3D& p0, const Vector3D& disp, const MeshElement *m,
  Vector3D* hitPoint) {

  // if the normal vector is zero triangle is degenerate
  if (same(m->n().x, 0.0) && same(m->n().y, 0.0) && same(m->n().z, 0.0)) {
    return 4;
  }

  // compute intersection of ray from p0 along disp with plane in which m is
  // located
  Vector3D w0 = p0 - m->v0();
  double a = -(m->n() * w0);
  double b = m->n() * disp;
  if (fabs(b) < GEOM_EPSILON) {  // our ray is parallel to triangle plane
    if (same(a, 0.0)) { // our ray is coplanar with the triangle
      return 3;
    } else {
      return 2;
    }
  }

  double r = a / b;
  if (r < 0) {  // if ray points away from triangle plane we won't hit it
    return 2;
  } else if (r > 1) {  // if the ray segment doesn't reach the plane we won't hit it
    return 1;
  }
  *hitPoint = p0 + r * disp;

  // now test that hitPoint is within the triangle
  // we use local variable for efficiency
  Vector3D w = *hitPoint - m->v0();
  Vector3D u = m->u();
  Vector3D v = m->v();
  double uu = u * u;
  double uv = u * v;
  double vv = v * v;
  double wu = w * u;
  double wv = w * v;
  double D = uv * uv - uu * vv;

  // compute and test parametric coords
  double s = (uv * wv - vv * wu) / D;
  if (s < 0.0 || s > 1.0) {        // hitPoint is outside m
    return 2;
  }
  double t = (uv * wu - uu * wv) / D;
  if (t < 0.0 || (s + t) > 1.0) { // hitPoint is outside m
    return 2;
  }
  return 0;  // hitPoint is in m
}












