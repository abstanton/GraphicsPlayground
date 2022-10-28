#include "CameraMovementSystem.h"

#include <components/Camera.h>
#include <platform/Input.h>

void CameraMovementSystem::onConfigure(ecs::Registry& registry) {
  registry.view<Camera>().each(
      [this](ecs::Entity ent, Camera& cam) { camera_entity = ent; });
}

void CameraMovementSystem::onUpdate(ecs::Registry& registry, float delta_time) {
  Camera& camera = registry.getComponent<Camera>(camera_entity);

  Input& input = Input::get();

  glm::vec2 mouse_movement = input.getMouseMovement();
  glm::vec2 scroll_offset = input.getScrollOffset();

  camera.processMouseMovement(mouse_movement.x, mouse_movement.y);
  camera.processMouseScroll(scroll_offset.y);
  camera.Sprint = input.getKeyState(Key::L_SHIFT) == KeyState::DOWN;
  if (input.getKeyState(Key::W) == KeyState::DOWN)
    camera.processKeyboard(FORWARD, delta_time);
  if (input.getKeyState(Key::S) == KeyState::DOWN)
    camera.processKeyboard(BACKWARD, delta_time);
  if (input.getKeyState(Key::A) == KeyState::DOWN)
    camera.processKeyboard(LEFT, delta_time);
  if (input.getKeyState(Key::D) == KeyState::DOWN)
    camera.processKeyboard(RIGHT, delta_time);
}