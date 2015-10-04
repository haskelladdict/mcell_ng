// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>
#include <stdexcept>

#include "geometry.hpp"


// MeshElement constructor
geom::MeshElement::MeshElement(const Vec3& av, const Vec3& bv, const Vec3& cv,
  MeshProp p) : a{av}, b{bv}, c{cv}, prop{p} {

  u = b - a;
  v = c - a;
  n = cross(u, v);
  if (n == geom::Vec3{}) {
    throw std::runtime_error("encountered degenerate MeshElement");
  }
  n_norm = normalize(n);
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
int geom::intersect(const Vec3& p0, const Vec3& disp, const MeshElement* m,
  Vec3* hitPoint) {

  // compute intersection of ray from p0 along disp with plane in which m is
  // located
  Vec3 w0 = p0 - m->a;
  double a = -(m->n * w0);
  double b = m->n * disp;
  if (fabs(b) < EPSILON) {  // our ray is parallel to triangle plane
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
  Vec3 w = *hitPoint - m->a;
  Vec3 u = m->u;
  Vec3 v = m->v;
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
