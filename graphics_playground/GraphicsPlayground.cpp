#include "GraphicsPlayground.h"

#include "Material.h"
#include "components/MeshRenderer.h"

GraphicsPlayground::GraphicsPlayground() {
  system_manager = std::make_unique<SystemManager>(registry);

  window = std::make_unique<Window>(1920, 1280, "Graphics Playground");
  InputManager& input_manager = InputManager::get();
  window->setMouseMovementCallback(
      [&](double x, double y) { input_manager.mouseMoveCallback(x, y); });
  window->setScrollOffsetCallback(
      [&](double x, double y) { input_manager.scrollCallback(x, y); });

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glEnable(GL_DEPTH_TEST);
}

void GraphicsPlayground::processInput(GLFWwindow* window, Camera& camera) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  camera.Sprint = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(FORWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(BACKWARD, delta_time);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(LEFT, delta_time);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(RIGHT, delta_time);
}

void GraphicsPlayground::run() {
  Renderer renderer(1920, 1280, glm::vec3(0.2, 0.2, 0.2));
  RenderSystem render_system(&renderer);
  system_manager->addSystem(&render_system);

  this->setup();
  this->load();

  system_manager->configure();
  while (!glfwWindowShouldClose(window->window_)) {
    glfwSwapBuffers(window->window_);
    glfwPollEvents();

    float current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_time;
    last_time = current_frame;

    system_manager->update(delta_time);
    system_manager->render();
  }
  glfwTerminate();
  return;
}
