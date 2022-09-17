#pragma once
#include "../resources/Material.h"
#include "../resources/Mesh.h"
#include "Transform.h"

using std::string;
struct MeshRenderer {
 public:
  string mesh_;
  Material material_comp_;
  // TODO Remove this
  Transform transform_;
  bool cast_shadows = true;

  MeshRenderer(){};
  MeshRenderer(string mesh, Material material)
      : mesh_(mesh), material_comp_(material) {}
  MeshRenderer(string mesh, Material material, Transform transform)
      : mesh_(mesh), material_comp_(material), transform_(transform) {}
};
