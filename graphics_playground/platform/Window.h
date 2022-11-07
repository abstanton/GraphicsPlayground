#pragma once

#include "../gpu/GPUBackend.h"
#include <GLFW/glfw3.h>

#include <functional>
#include <vector>


using MouseMoveCallback = std::function<void(double, double)>;
using ScrollOffsetCallback = std::function<void(double, double)>;
using ResizeCallback = std::function<void(int, int)>;

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
  Window(int width, int height, const char* title, bool hide_cursor = true);

  virtual ~Window();

  void pollEvents();
  void swapBuffers();

  bool shouldClose();

  void setMouseMovementCallback(MouseMoveCallback);
  void setScrollOffsetCallback(ScrollOffsetCallback);
  void setResizeCallback(ResizeCallback);
  KeyState getKeyState(Key key) const;

 private:
  GLFWwindow* window_;
  MouseMoveCallback mouse_callback_;
  ScrollOffsetCallback scroll_callback_;
  ResizeCallback resize_callback_;

  int width;
  int height;

  static void mouseMoveCallback(GLFWwindow* window, double xposIn,
                                double yposIn);
  static void scrollCallback(GLFWwindow* window, double xoffset,
                             double yoffset);

  static void resizeCallback(GLFWwindow* window, int width, int height);

  KeyState processGlfwKey(int glfw_key) const;
};
