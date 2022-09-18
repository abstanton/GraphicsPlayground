#include "RotateSystem.h"

#include <components/MeshRenderer.h>

#include "../components/RotateComponent.h"

void RotateSystem::onUpdate(ecs::Registry& registry, float delta_time) {
  registry.view<MeshRenderer, Rotate>().each(
      [&](ecs::Entity ent, MeshRenderer& mr, Rotate& rot) {
        float dt = 0.000178461f;
        mr.transform_.setRotation(
            mr.transform_.euler() +
            (glm::vec3(rot.rate_x, rot.rate_y, rot.rate_z) * dt));
      });
}
