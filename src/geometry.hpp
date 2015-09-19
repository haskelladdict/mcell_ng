// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <array>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "species.hpp"
#include "util.hpp"

namespace geom {

// this epsilon is used for geometrical comparison. Anything smaller than that
// is assumed to be identical.
// FIXME: This is currently chosen arbitrarily and requires more thinking.
const double EPSILON = 1e-12;
const double EPSILON_2 = 1e-24;


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

  Vec3 a, b, c;   // triangle vertices
  Vec3 u, v;      // triangle vectors
  Vec3 n;         // normal vector
  Vec3 n_norm;    // normalized normal vector - precomputed for efficiency

private:
  MeshProp prop_;  // properties of this element
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
// NOTE: Since MeshElements are shared between up to two Tets, they can either
// be oriented with their normal to the outside or with their normal to the
// inside (indicated by o1, o2, ... o4). If they are oriented normal in we have
// to take that into account when checking for collisions.
struct Tet {

  const static size_t unset = std::numeric_limits<size_t>::max();

  // indices of MeshElements
  std::array<size_t, 4> m{{unset, unset, unset, unset}};

  // indices of neighboring Tets
  std::array<size_t, 4> t{{unset, unset, unset, unset}};

  // orientation of MeshElements with respect to Tet;
  //  1 indicates normal out, -1 normal in
  std::array<int, 4> o{{0, 0, 0, 0 }};
};
using Tets = Rvector<Tet>;

// tetFaces lists the indices of all triangles that make up the four
// faces of a tet
const Rvector<Rvector<size_t>> tetFaces{Rvector<size_t>{0, 2, 1}
                                       ,Rvector<size_t>{0, 1, 3}
                                       ,Rvector<size_t>{1, 2, 3}
                                       ,Rvector<size_t>{2, 0, 3}};

}

#endif
