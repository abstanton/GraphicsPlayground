#pragma once
#include "../ecs/Registry.hpp"

class System {
 public:
  virtual void onConfigure(ecs::Registry& registry) {}
  virtual void onPreUpdate(ecs::Registry& registry) {}
  virtual void onUpdate(ecs::Registry& registry, float delta_time) {}
  virtual void onPostUpdate(ecs::Registry& registry) {}
  virtual void onRender(ecs::Registry& registry) {}
  virtual void onExit(ecs::Registry& registry) {}
};
