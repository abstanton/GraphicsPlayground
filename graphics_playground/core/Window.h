#pragma once

#include <functional>
#include <vector>

#include "../gpu/GPUBackend.h"
#include <GLFW/glfw3.h>

using MouseMoveCallback = std::function<void(double, double)>;
using ScrollOffsetCallback = std::function<void(double, double)>;

class GraphicsPlayground;

class Window {
  friend GraphicsPlayground;

 public:
  Window(int width, int height, const char* title);

  void setMouseMovementCallback(MouseMoveCallback);
  void setScrollOffsetCallback(ScrollOffsetCallback);

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
};
