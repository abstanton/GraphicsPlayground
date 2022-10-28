#pragma once


#include <functional>
#include <vector>

#include "../gpu/GPUBackend.h"
#include <GLFW/glfw3.h>

using MouseMoveCallback = std::function<void(double, double)>;
using ScrollOffsetCallback = std::function<void(double, double)>;

class GraphicsPlayground;

enum Key {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  T,
  U,
  V,
  W,
  X,
  Y,
  Z,
  N_0,
  N_1,
  N_2,
  N_3,
  N_4,
  N_5,
  N_6,
  N_7,
  N_8,
  N_9,
  SPACE,
  ENTER,
  L_SHIFT,
  R_SHIFT,
  L_CTRL,
  R_CTRL,
  TAB,
};

enum KeyState { DOWN, UP };

class Window {
  friend GraphicsPlayground;

 public:
  Window(int width, int height, const char* title);

  void setMouseMovementCallback(MouseMoveCallback);
  void setScrollOffsetCallback(ScrollOffsetCallback);
  KeyState getKeyState(Key key) const;

 private:
  GLFWwindow* window_;
  MouseMoveCallback mouse_callback_;
  ScrollOffsetCallback scroll_callback_;

  int width;
  int height;

  static void mouseMoveCallback(GLFWwindow* window, double xposIn,
                                double yposIn);
  static void scrollCallback(GLFWwindow* window, double xoffset,
                             double yoffset);

  KeyState processGlfwKey(int glfw_key) const;
};
