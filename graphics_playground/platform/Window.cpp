#include "Window.h"

Window::Window(int width, int height, const char* title, bool hide_cursor)
    : width(width), height(height) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwSetWindowUserPointer(window_, this);
  glfwSetCursorPosCallback(window_, &Window::mouseMoveCallback);
  glfwSetScrollCallback(window_, &Window::scrollCallback);
  glfwSetFramebufferSizeCallback(window_, &Window::resizeCallback);

  glfwMakeContextCurrent(window_);

  if (hide_cursor) {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return;
  }
}

void Window::resizeCallback(GLFWwindow* window, int x_pos, int y_pos) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
      ->resize_callback_(x_pos, y_pos);
}

void Window::mouseMoveCallback(GLFWwindow* window, double x_pos, double y_pos) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
      ->mouse_callback_(x_pos, y_pos);
}

void Window::scrollCallback(GLFWwindow* window, double x_offset,
                            double y_offset) {
  static_cast<Window*>(glfwGetWindowUserPointer(window))
      ->scroll_callback_(x_offset, y_offset);
}

Window::~Window() { glfwTerminate(); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::swapBuffers() { glfwSwapBuffers(window_); }

bool Window::shouldClose() { return glfwWindowShouldClose(window_); }

void Window::setMouseMovementCallback(MouseMoveCallback func) {
  mouse_callback_ = func;
}

void Window::setScrollOffsetCallback(ScrollOffsetCallback func) {
  scroll_callback_ = func;
}

void Window::setResizeCallback(ResizeCallback func) { resize_callback_ = func; }

KeyState Window::processGlfwKey(int glfw_key) const {
  int glfw_state = glfwGetKey(window_, glfw_key);
  switch (glfw_state) {
    case GLFW_PRESS:
      return KeyState::DOWN;
    default:
      return KeyState::UP;
  }
}

KeyState Window::getKeyState(Key key) const {
  int glfw_key;
  switch (key) {
    case Key::A:
      glfw_key = GLFW_KEY_A;
      break;
    case Key::D:
      glfw_key = GLFW_KEY_D;
      break;
    case Key::S:
      glfw_key = GLFW_KEY_S;
      break;
    case Key::W:
      glfw_key = GLFW_KEY_W;
      break;
    case Key::SPACE:
      glfw_key = GLFW_KEY_SPACE;
      break;
    case Key::L_SHIFT:
      glfw_key = GLFW_KEY_LEFT_SHIFT;
      break;
    // Default to enter, seems almost reasonable
    default:
      glfw_key = GLFW_KEY_ENTER;
  }
  return processGlfwKey(glfw_key);
}
