#include "RenderSystem.h"

#include "../components/DirectionLight.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"
#include "../scene/Camera.h"

namespace gp {

RenderSystem::RenderSystem(Renderer* renderer) { renderer_ = renderer; }

void RenderSystem::onConfigure(World& world) {}

void RenderSystem::onRender(World& world) {
  std::vector<TransformAnd<MeshRenderer>> mesh_renderers;
  world.view<MeshRenderer, Transform>().each(
      [&](ecs::Entity e, MeshRenderer& mr, Transform& t) {
        std::optional<GameObject> gameObject = world.getGameObject(e);
        if (gameObject.has_value())
          mesh_renderers.push_back({mr, gameObject->getGlobalTransform()});
      });
  std::vector<TransformAnd<PointLight>> point_pairs;
  world.view<PointLight, Transform>().each(
      [&](ecs::Entity, PointLight& pc, Transform& transform) {
        point_pairs.push_back({pc, transform});
      });
  std::vector<DirectionLight> direction_lights;
  world.view<DirectionLight>().each(
      [&](ecs::Entity, DirectionLight& dr) { direction_lights.push_back(dr); });

  renderer_->draw(world.getCamera(), mesh_renderers, point_pairs,
                  direction_lights);
}

void RenderSystem::onExit(World& world) {}

}  // namespace gp