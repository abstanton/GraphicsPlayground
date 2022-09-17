#include "Window.h"

void Window::mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
      ->mouse_callback_(x_pos, y_pos);
}

void Window::scrollCallback(GLFWwindow* window, double x_offset,
                            double y_offset) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
      ->scroll_callback_(x_offset, y_offset);
}

Window::Window(int width, int height, const char* title)
    : width(width), height(height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  window_ = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, &Window::mouseMoveCallback);
  glfwSetScrollCallback(window_, &Window::scrollCallback);
  glfwSetFramebufferSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        gpu::Backend::get()->setViewport(0, 0, width, height);
      });

  glfwMakeContextCurrent(window_);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::setMouseMovementCallback(MouseMoveCallback func) {
  mouse_callback_ = func;
}

void Window::setScrollOffsetCallback(ScrollOffsetCallback func) {
  scroll_callback_ = func;
}
