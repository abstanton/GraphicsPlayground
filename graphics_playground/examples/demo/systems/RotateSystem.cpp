#include "RotateSystem.h"

#include <iostream>

#include "../../../components/MeshRenderer.h"
#include "../components/RotateComponent.h"
void RotateSystem::onUpdate(ecs::Registry& registry, float delta_time) {
  registry.view<MeshRenderer, Rotate>().each(
      [&](ecs::Entity ent, MeshRenderer& mr, Rotate& rot) {
        std::cout << "DT" << delta_time << std::endl;
        float dt = 0.0178461f;
        mr.transform_.euler.x += rot.rate_x * dt;
        mr.transform_.euler.y += rot.rate_y * dt;
        mr.transform_.euler.z += rot.rate_z * dt;
      });
}