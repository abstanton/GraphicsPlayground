#include "CameraMovementSystem.h"
#include "../../../core/managers/InputManager.h"
#include "../../../components/Camera.h"

void CameraMovementSystem::onConfigure(ecs::Registry& registry) {
	registry.view<Camera>().each([this](ecs::Entity ent, Camera& cam) {
		camera_entity = ent;
		});
}

void CameraMovementSystem::onUpdate(ecs::Registry& registry, float delta_time) {
	Camera& camera = registry.getComponent<Camera>(camera_entity);

	glm::vec2 mouse_movement = InputManager::get().getMouseMovement();
	glm::vec2 scroll_offset = InputManager::get().getScrollOffset();
	
	camera.processMouseMovement(mouse_movement.x, mouse_movement.y);
	camera.processMouseScroll(scroll_offset.y);
}