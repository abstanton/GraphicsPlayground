#pragma once
#include "MeshRenderer.h"
#include "Transform.h"
#include "glm/glm.hpp"

namespace gp {

struct Instances {
  MeshRenderer* mesh_renderer;
  // max instances so we can alloc one SSBO and just update pos/scales
  unsigned int max_instances;
  // how to associate these with this component in the renderer? use the entity
  // id?
  std::vector<Transform> transforms;
}

}