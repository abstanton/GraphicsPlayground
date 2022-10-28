#pragma once
#include <glm/glm.hpp>
#include <map>

#include "Window.h"

class GraphicsPlayground;

class Input {
  friend GraphicsPlayground;

 public:
  static Input& get() {
    static Input input_manager;
    return input_manager;
  }

  void setWindow(const Window* window) { window_ = window; }
  KeyState getKeyState(Key) const;
  glm::vec2 getMouseMovement();
  glm::vec2 getScrollOffset();

 private:
  Input() {}

  bool first_mouse_movement = true;
  double last_mouse_pos[2] = {0, 0};

  const Window* window_ = nullptr;
  struct InputState {
    double scroll_offset[2];
    double mouse_position[2];
  };

  void mouseMoveCallback(double x_pos, double y_pos) {
    last_mouse_pos[0] = input_state_.mouse_position[0];
    last_mouse_pos[1] = input_state_.mouse_position[1];
    input_state_.mouse_position[0] = x_pos;
    input_state_.mouse_position[1] = y_pos;
  }

  void scrollCallback(double x_offset, double y_offset) {
    input_state_.scroll_offset[0] = x_offset;
    input_state_.scroll_offset[1] = y_offset;
  }

  InputState input_state_{};
};
