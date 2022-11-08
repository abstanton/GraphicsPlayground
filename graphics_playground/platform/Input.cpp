#include "Input.h"

glm::vec2 Input::getMouseMovement() {
  auto& inst = Input::get();
  double xpos = inst.input_state_.mouse_position[0];
  double ypos = inst.input_state_.mouse_position[1];

  double xoffset = xpos - inst.last_mouse_pos[0];
  double yoffset = inst.last_mouse_pos[1] -
                   ypos;  // reversed since y-coordinates go from bottom to top

  inst.last_mouse_pos[0] = xpos;
  inst.last_mouse_pos[1] = ypos;

  return {xoffset, yoffset};
}

glm::vec2 Input::getScrollOffset() {
  auto& inst = Input::get();
  glm::vec2 output = {inst.input_state_.scroll_offset[0],
                      inst.input_state_.scroll_offset[1]};
  inst.input_state_.scroll_offset[0] = 0;
  inst.input_state_.scroll_offset[1] = 0;
  return output;
}

KeyState Input::getKeyState(Key key) {
  auto& inst = Input::get();
  if (inst.window_ == nullptr) {
    return KeyState::UP;
  }
  return inst.window_->getKeyState(key);
}