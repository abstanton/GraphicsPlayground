#include "RenderSystem.h"

#include "../components/Camera.h"
#include "../components/DirectionLight.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"

RenderSystem::RenderSystem(Renderer* renderer) { renderer_ = renderer; }

void RenderSystem::onConfigure(ecs::Registry& registry) {
  registry.view<Camera>().each(
      [this](ecs::Entity ent, Camera& cam) { camera_entity_ = ent; });
}

void RenderSystem::onRender(ecs::Registry& registry) {
  std::vector<MeshPair> mesh_renderers;
  registry.view<MeshRenderer, Transform>().each(
      [&](ecs::Entity, MeshRenderer& mr, Transform& t) {
        mesh_renderers.push_back({mr, t});
      });
  std::vector<PointPair> point_pairs;
  registry.view<PointLight, Transform>().each(
      [&](ecs::Entity, PointLight& pc, Transform& transform) {
        point_pairs.push_back({pc, transform});
      });
  std::vector<DirectionLight> direction_lights;
  registry.view<DirectionLight>().each(
      [&](ecs::Entity, DirectionLight& dr) { direction_lights.push_back(dr); });
  Camera camera = registry.getComponent<Camera>(camera_entity_);

  renderer_->draw(camera, mesh_renderers, point_pairs, direction_lights);
}

void RenderSystem::onExit(ecs::Registry& registry) {}