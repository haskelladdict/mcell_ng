// Copyright 2015 Markus Dittrich
// Licensed under BSD license, see LICENSE file for details

#include <cassert>

#include "geometry.hpp"

// MeshElement constructor
MeshElement::MeshElement(const Vec3& a, const Vec3& b, const Vec3& c,
  MeshPropPtr prop) : a_{a}, b_{b}, c_{c}, prop_{prop} {

  u_ = b - a;
  v_ = c - a;
  n_ = cross(u_, v_);
  nn_ = normalize(n_);
}

// helper function for creating a rectangular geometry primitive
Mesh create_rectangle(const Vec3& llc, const Vec3& urc) {

  // for rectangle to be well formed llc needs to be smaller than urc for x, y and z
  assert(llc.x < urc.x && llc.y < urc.y && llc.z < urc.z);

  Vec3 diag = urc - llc;

  auto v0 = Vec3{llc};
  auto v1 = Vec3{llc} + Vec3{diag.x, 0.0, 0.0};
  auto v2 = Vec3{llc} + Vec3{0.0, diag.y, 0.0};
  auto v3 = Vec3{llc} + Vec3{0.0, 0.0, diag.z};
  auto v4 = Vec3{llc} + Vec3{diag.x, diag.y, 0.0};
  auto v5 = Vec3{llc} + Vec3{diag.x, 0.0, diag.z};
  auto v6 = Vec3{llc} + Vec3{0.0, diag.y, diag.z};
  auto v7 = Vec3{urc};

  Mesh mesh;
  auto meshProp = std::make_shared<MeshProp>();
  meshProp->name = "cube";
  mesh.reserve(8);
  mesh.emplace_back(MeshElement(v0, v1, v5, meshProp));
  mesh.emplace_back(MeshElement(v0, v5, v3, meshProp));
  mesh.emplace_back(MeshElement(v1, v4, v7, meshProp));
  mesh.emplace_back(MeshElement(v1, v7, v5, meshProp));
  mesh.emplace_back(MeshElement(v4, v2, v6, meshProp));
  mesh.emplace_back(MeshElement(v4, v6, v7, meshProp));
  mesh.emplace_back(MeshElement(v2, v0, v3, meshProp));
  mesh.emplace_back(MeshElement(v2, v3, v6, meshProp));
  mesh.emplace_back(MeshElement(v5, v7, v6, meshProp));
  mesh.emplace_back(MeshElement(v5, v6, v3, meshProp));
  mesh.emplace_back(MeshElement(v0, v2, v1, meshProp));
  mesh.emplace_back(MeshElement(v1, v2, v4, meshProp));
  return mesh;
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
int intersect(const Vec3& p0, const Vec3& disp, const MeshElement& m,
  Vec3* hitPoint) {

  // if the normal vector is zero triangle is degenerate
  if (same(m.n().x, 0.0) && same(m.n().y, 0.0) && same(m.n().z, 0.0)) {
    return 4;
  }

  // compute intersection of ray from p0 along disp with plane in which m is
  // located
  Vec3 w0 = p0 - m.a();
  double a = -(m.n() * w0);
  double b = m.n() * disp;
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
  Vec3 w = *hitPoint - m.a();
  Vec3 u = m.u();
  Vec3 v = m.v();
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












