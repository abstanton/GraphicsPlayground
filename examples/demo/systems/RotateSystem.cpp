#include "RotateSystem.h"

#include <components/MeshRenderer.h>

#include "../components/RotateComponent.h"

using namespace gp;

void RotateSystem::onUpdate(gp::World& world, float delta_time) {
  world.view<MeshRenderer, Transform, Rotate>().each(
      [&](ecs::Entity ent, MeshRenderer& mr, Transform& tr, Rotate& rot) {
        tr.setRotation(
            tr.euler() +
            (glm::vec3(rot.rate_x, rot.rate_y, rot.rate_z) * delta_time));
      });
}
