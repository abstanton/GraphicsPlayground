#include "RenderSystem.h"
#include "../components/MeshRenderer.h"
#include "../components/PointLight.h"
#include "../components/DirectionLight.h"
#include "../components/Camera.h"

RenderSystem::RenderSystem(Renderer* renderer) {
	renderer_ = renderer;
}

void RenderSystem::onConfigure(ecs::Registry& registry) {
	registry.view<Camera>().each([this](ecs::Entity ent, Camera& cam) {
		camera_entity_ = ent;
		});
}

void RenderSystem::onRender(ecs::Registry& registry) {
	std::vector<MeshRenderer> mesh_renderers;
	registry.view<MeshRenderer>().each([&] (ecs::Entity, MeshRenderer& mr) {
		mesh_renderers.push_back(mr);
		});
	std::vector<PointLight> point_lights;
	std::vector<DirectionLight> direction_lights;
	registry.view<DirectionLight>().each([&] (ecs::Entity, DirectionLight& dr) {
		direction_lights.push_back(dr);
		});
	Camera camera = registry.getComponent<Camera>(camera_entity_);
	
	renderer_->draw(
		camera,
		mesh_renderers,
		point_lights,
		direction_lights
	);
}

void RenderSystem::onExit(ecs::Registry& registry) {
}