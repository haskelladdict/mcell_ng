// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "species.hpp"
#include "util.hpp"


// this epsilon is used for geometrical comparison. Anything smaller than that
// is assumed to be identical.
// FIXME: This is currently chosen arbitrarily and requires more thinking.
const double GEOM_EPSILON = 1e-12;
const double GEOM_EPSILON_2 = 1e-24;

// forward declaration
class Mesh;


// MeshProperty defines the behavior of mesh elements toward diffusing volume
// or surface molecules
enum class MeshProperty {
  reflective,
  absorptive,
  transparent
};


// MeshElementProperty defines the properties of a single mesh element
struct MeshElementProperty {
  std::string name;
  MeshProperty prop;
  MolSpecies* spec;
};


// triangle describes a single triangle on a mesh. It consists of the indexes
// of the vertices making up the triangle.
class MeshElement{

public:

  MeshElement(uint64_t i0, uint64_t i1, uint64_t i2, const Mesh& parent,
    const MeshElementProperty* prop = nullptr);

  // indices of this MeshElement vertices in Mesh's vertex list
  uint64_t i0() const {
    return vert0_;
  }

  uint64_t i1() const {
    return vert1_;
  }

  uint64_t i2() const {
    return vert2_;
  }

  // coordinate vectors of MeshElement: u, v and n
  const Vector3D& u() const {
    return u_;
  }

  const Vector3D& v() const {
    return v_;
  }

  const Vector3D& n() const {
    return normal_;
  }

  // normalized normal vector
  const Vector3D& n_norm() const {
    return normal_norm_;
  }

  // coordinates MeshElement vertices
  const Vector3D& v0() const;
  const Vector3D& v1() const;
  const Vector3D& v2() const;

  void add_meshElementProperty(const MeshElementProperty* prop);
  bool delete_meshElementProperty(const MeshElementProperty* prop);


private:

  uint64_t vert0_;
  uint64_t vert1_;
  uint64_t vert2_;
  Vector3D u_;           // triangle vectors
  Vector3D v_;
  Vector3D normal_;      // normal vector
  Vector3D normal_norm_; // normalized normal vector - precomputed for efficiency
  const Mesh& mesh_;     // reference to parent mesh
  Vec<const MeshElementProperty*> props_;
};
using MeshElementPtr = std::unique_ptr<MeshElement>;
using MeshElements = Vec<MeshElementPtr>;

// Mesh describes a contiguous (closed or open) surface consisting of
// a collection of vertices and triangular connections between them.
class Mesh {

public:

  Mesh(std::string name);

  friend std::ostream& operator<<(std::ostream& os, const Mesh& m);

  void add_vertex(Vector3D&& x);

  const Vector3D& get_vertex(size_t i) const {
    return verts_[i];
  }

  MeshElement* add_meshElement(uint64_t i0, uint64_t i1, uint64_t i2,
    const MeshElementProperty* props = nullptr);

  MeshElement* get_meshElement(size_t i) const {
    return meshElems_[i].get();
  }

  const MeshElements& get_meshElements() const {
    return meshElems_;
  }

  const

  auto num_meshElements() const {
    return meshElems_.size();
  }


private:

  std::string name_;
  Vec<Vector3D> verts_;
  MeshElements meshElems_;
};
using MeshPtr = std::unique_ptr<Mesh>;

// provide operator<< for Mesh for debugging purposes
std::ostream& operator<<(std::ostream& os, const Mesh& m);

// helper function for creating a rectangular geometry primitive. This function
// returns a vector with pointers to all MeshElements.
Vec<MeshElement*> create_rectangle(Mesh* mesh, const Vector3D& llc, const Vector3D& urc);

// intersect tests for ray triangle intersections. Possible return values are
//  0: triangle and ray segment intersect, in this case hitPoint contains the
//     location of the intersection point
//  1: triangle and ray intersect but ray segment does not reach the triangle
//  2: triangle and ray do not intersect
//  3: ray and triangle are co-planar
//  4: triangle is degenerate
int intersect(const Vector3D& p0, const Vector3D& disp, const MeshElement *m,
  Vector3D* hitPoint);

#endif




