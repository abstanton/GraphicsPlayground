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

  renderer = std::make_unique<Renderer>(1920, 1280, glm::vec3(0.2, 0.2, 0.2));
  window->setResizeCallback(
      [this](int x, int y) { renderer->resizeViewport(x, y); });
}

void GraphicsPlayground::run() {
  RenderSystem render_system(renderer.get());
  system_manager->addSystem(&render_system);

  this->setup();
  this->load();

  system_manager->configure();
  while (!window->shouldClose()) {
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
