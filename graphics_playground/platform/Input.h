#pragma once
#include <glm/glm.hpp>
#include <map>

#include "../common/Singleton.hpp"
#include "Window.h"

namespace gp {

class GraphicsPlayground;

class Input : public SingletonMixIn<Input> {
  friend GraphicsPlayground;

 public:
  static void setWindow(const Window* window) {
    auto& inst = Input::get();
    inst.window_ = window;
  }
  static KeyState getKeyState(Key);
  static glm::vec2 getMouseMovement();
  static glm::vec2 getScrollOffset();

 private:
  // Input() {}

  bool first_mouse_movement = true;
  double last_mouse_pos[2] = {0, 0};

  const Window* window_ = nullptr;
  struct InputState {
    double scroll_offset[2];
    double mouse_position[2];
  };

  static void mouseMoveCallback(double x_pos, double y_pos) {
    auto& inst = Input::get();
    inst.last_mouse_pos[0] = inst.input_state_.mouse_position[0];
    inst.last_mouse_pos[1] = inst.input_state_.mouse_position[1];
    inst.input_state_.mouse_position[0] = x_pos;
    inst.input_state_.mouse_position[1] = y_pos;
  }

  static void scrollCallback(double x_offset, double y_offset) {
    auto& inst = Input::get();
    inst.input_state_.scroll_offset[0] = x_offset;
    inst.input_state_.scroll_offset[1] = y_offset;
  }

  InputState input_state_{};
};

}  // namespace gp