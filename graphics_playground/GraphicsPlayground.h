#pragma once
#include "core/Window.h"
#include "core/ecs/Registry.hpp"
#include "core/managers/InputManager.h"
#include "core/managers/ResourceManager.h"
#include "core/managers/SystemManager.h"
#include "gpu/gl/GlShader.h"
#include "renderer/Renderer.h"
#include "systems/RenderSystem.h"

class GraphicsPlayground {
 public:
  GraphicsPlayground();

  void run();

  virtual void setup() = 0;
  virtual void load() = 0;

 protected:
  float delta_time;
  float last_time;

  std::unique_ptr<Window> window;
  std::unique_ptr<SystemManager> system_manager;
  ResourceManager& resource_manager_;

  ecs::Registry registry;

  // TODO: Improve
  void processInput(GLFWwindow* window, Camera& camera);
};
