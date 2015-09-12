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


// MeshProp describe properties of MeshElements. Each MeshElement can only have
// one of MeshProp. Transparent, absorptive, and reflective mesh elements are
// transparent, absorptive, and reflective toward diffusing volume molecules,
// respectively. Translucent meshes have a certain probability of letting a
// molecule through.
enum class MeshProp {
      transparent
    , absorptive
    , reflective
    , translucent
};


// MeshElement describes a single triangle on a mesh. It consists of the
// triangle vertices and also the triangles uv and normal vectors.
class MeshElement {

public:

  MeshElement(const Vec3& a, const Vec3& b, const Vec3& c,
    MeshProp prop = MeshProp::transparent);

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

  MeshProp prop() const {
    return prop_;
  }

private:

  Vec3 a_, b_, c_; // triangle vertices
  Vec3 u_, v_;     // triangle vectors
  Vec3 n_;         // normal vector
  Vec3 nn_;        // normalized normal vector - precomputed for efficiency
  MeshProp prop_;   // properties of this element
};

using Mesh = Rvector<MeshElement>;

// create_rectangle is a helper function for generating a rectangular geometry
// primitive. This function returns a vector with pointers to all MeshElements.
//Mesh create_rectangle(const Vec3& llc, const Vec3& urc, MeshPropPtr prop = nullptr);

// intersect tests for ray triangle intersections. Possible return values are
//  0: triangle and ray segment intersect, in this case hitPoint contains the
//     location of the intersection point
//  1: triangle and ray intersect but ray segment does not reach the triangle
//  2: triangle and ray do not intersect
//  3: ray and triangle are co-planar
//  4: triangle is degenerate
int intersect(const Vec3& p0, const Vec3& disp, const MeshElement& m,
  Vec3* hitPoint);


// Tet describes a tetrahedron consisting of four triangular MeshElements via
// indices m1 .. m4 into the Mesh vector of MeshElements. In addition, a Tet
// knows the indices t1 .. t4 to its neighboring Tet elements. A Tet index of
// unset indicates that there is no neighboring Tet and we're thus at the
// edge of the model geometry.
class Tet {

public:

  const size_t unset = std::numeric_limits<size_t>::max();

  size_t m1, m2, m3, m4;   // indices of MeshElements
  size_t t1, t2, t3, t4;   // indices of neighboring Tets
};

using Tets = Rvector<Tet>;


#endif
