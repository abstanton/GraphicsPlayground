#include "GraphicsPlayground.h"

#include <iostream>

#include "components/MeshRenderer.h"
#include "resources/core/Material.h"

GraphicsPlayground::GraphicsPlayground()
    : resource_manager_(ResourceManager::get()) {
  system_manager = std::make_unique<SystemManager>(registry);

  window = std::make_unique<Window>(1920, 1280, "Graphics Playground");
  Input& input_manager = Input::get();
  input_manager.setWindow(window.get());
  window->setMouseMovementCallback(
      [&](double x, double y) { input_manager.mouseMoveCallback(x, y); });
  window->setScrollOffsetCallback(
      [&](double x, double y) { input_manager.scrollCallback(x, y); });
}

void GraphicsPlayground::run() {
  // TODO: Refactor resolution into window, add renderer resize callback to
  // window resize callback
  Renderer renderer(1920, 1280, glm::vec3(0.2, 0.2, 0.2));
  RenderSystem render_system(&renderer);
  system_manager->addSystem(&render_system);

  this->setup();
  this->load();

  system_manager->configure();
  while (!window->shouldClose()) {
    // TODO Refactor this into window
    window->swapBuffers();
    window->pollEvents();

    float current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_time;
    last_time = current_frame;

    system_manager->update(delta_time);
    system_manager->render();
  }

  return;
}
