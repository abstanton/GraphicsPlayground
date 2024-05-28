#include "CameraMovementSystem.h"

#include <platform/Input.h>
#include <scene/Camera.h>

using namespace gp;

void CameraMovementSystem::onUpdate(gp::World& world, float delta_time) {
  Camera& camera = world.getCamera();

  glm::vec2 mouse_movement = Input::getMouseMovement();
  glm::vec2 scroll_offset = Input::getScrollOffset();

  camera.processMouseMovement(mouse_movement.x, mouse_movement.y);
  camera.processMouseScroll(scroll_offset.y);
  camera.Sprint = Input::getKeyState(Key::L_SHIFT) == KeyState::DOWN;
  if (Input::getKeyState(Key::W) == KeyState::DOWN)
    camera.processKeyboard(FORWARD, delta_time);
  if (Input::getKeyState(Key::S) == KeyState::DOWN)
    camera.processKeyboard(BACKWARD, delta_time);
  if (Input::getKeyState(Key::A) == KeyState::DOWN)
    camera.processKeyboard(LEFT, delta_time);
  if (Input::getKeyState(Key::D) == KeyState::DOWN)
    camera.processKeyboard(RIGHT, delta_time);
}