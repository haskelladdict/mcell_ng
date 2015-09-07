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


// MeshProp described the properties of a given MeshElement, e.g. which object
// it belongs to.
struct MeshProp {
  std::string name;   // name of object
};
using MeshPropPtr = std::shared_ptr<MeshProp>;


// MeshElement describes a single triangle on a mesh. It consists of the
// triangle vertices and also the triangles uv and normal vectors.
class MeshElement{

public:

  MeshElement(const Vec3& a, const Vec3& b, const Vec3& c,
    MeshPropPtr prop = nullptr);

  const Vec3& a() const {
    return a_;
  }

  const Vec3& b() const {
    return b_;
  }

  const Vec3& c() const {
    return c_;
  }

  const Vec3& u() const {
    return u_;
  }

  const Vec3& v() const {
    return v_;
  }

  const Vec3& n() const {
    return n_;
  }

  const Vec3& n_norm() const {
    return nn_;
  }

private:

  Vec3 a_, b_, c_; // triangle vertices
  Vec3 u_, v_;     // triangle vectors
  Vec3 n_;         // normal vector
  Vec3 nn_;        // normalized normal vector - precomputed for efficiency
  MeshPropPtr prop_; // properties of this element
};

using Mesh= Vec<MeshElement>;

// create_rectangle is a helper function for generating a rectangular geometry
// primitive. This function returns a vector with pointers to all MeshElements.
Mesh create_rectangle(const Vec3& llc, const Vec3& urc);

// intersect tests for ray triangle intersections. Possible return values are
//  0: triangle and ray segment intersect, in this case hitPoint contains the
//     location of the intersection point
//  1: triangle and ray intersect but ray segment does not reach the triangle
//  2: triangle and ray do not intersect
//  3: ray and triangle are co-planar
//  4: triangle is degenerate
int intersect(const Vec3& p0, const Vec3& disp, const MeshElement& m,
  Vec3* hitPoint);

#endif




