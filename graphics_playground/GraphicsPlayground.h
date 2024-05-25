#pragma once
#include "ecs/Registry.hpp"
#include "platform/Input.h"
#include "platform/Window.h"
#include "renderer/Renderer.h"
#include "resources/ResourceManager.h"
#include "systems/SystemManager.h"
#include "systems/core/RenderSystem.h"

namespace gp {

class GraphicsPlayground {
 public:
  GraphicsPlayground();

  void run();

  virtual void setup() = 0;
  virtual void load() = 0;

 protected:
  std::unique_ptr<Window> window;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<SystemManager> system_manager;

  ecs::Registry registry;

 private:
  float delta_time;
  float last_time;
};

}  // namespace gp