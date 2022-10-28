#pragma once
#include "../resources/core/Material.h"
#include "../resources/core/Mesh.h"
#include "Transform.h"

using std::string;
struct MeshRenderer {
 public:
  string mesh_;
  Material material_comp_;
  bool cast_shadows = true;

  MeshRenderer(){};
  MeshRenderer(string mesh, Material material)
      : mesh_(mesh), material_comp_(material) {}
};
