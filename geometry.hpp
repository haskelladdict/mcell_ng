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

  MeshElement(uint64_t v1, uint64_t v2, uint64_t v3, const Mesh& parent,
    const MeshElementProperty* prop = nullptr);

  uint64_t v1() const {
    return vert1_;
  }

  uint64_t v2() const {
    return vert2_;
  }

  uint64_t v3() const {
    return vert3_;
  }

  void add_meshElementProperty(const MeshElementProperty* prop);
  bool delete_meshElementProperty(const MeshElementProperty* prop);


private:

  uint64_t vert1_;
  uint64_t vert2_;
  uint64_t vert3_;
  const Mesh& parent_;         // reference to parent mesh
  Vec<const MeshElementProperty*> props_;
};
using MeshElementPtr = std::unique_ptr<MeshElement>;


// Mesh describes a contiguous (closed or open) surface consisting of
// a collection of vertices and triangular connections between them.
class Mesh {

public:

  Mesh(std::string name);

  friend std::ostream& operator<<(std::ostream& os, const Mesh& m);

  void add_vertex(Vector3D&& x);
  MeshElement* add_meshElement(uint64_t v1, uint64_t v2, uint64_t v3,
    const MeshElementProperty* props = nullptr);

  MeshElement* get_meshElement(size_t i) const {
    return meshElems_[i].get();
  }

  auto num_meshElements() const {
    return meshElems_.size();
  }


private:

  std::string name_;
  Vec<Vector3D> verts_;
  Vec<MeshElementPtr> meshElems_;
};
using MeshPtr = std::unique_ptr<Mesh>;

// provide operator<< for Mesh for debugging purposes
std::ostream& operator<<(std::ostream& os, const Mesh& m);

// helper function for creating a rectangular geometry primitive. This function
// returns a vector with pointers to all MeshElements.
Vec<MeshElement*> create_rectangle(Mesh* mesh, const Vector3D& llc, const Vector3D& urc);


#endif




