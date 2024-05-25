#pragma once
#include "../resources/core/Material.h"
#include "../resources/core/Mesh.h"
#include "Transform.h"

namespace gp {

using std::string;
struct MeshRenderer {
  string mesh;
  Material material;
  bool cast_shadows = true;

  MeshRenderer(){};
  MeshRenderer(string mesh, Material material)
      : mesh(mesh), material(material) {}
};

}