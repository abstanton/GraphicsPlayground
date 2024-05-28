#pragma once
#include "../ecs/Registry.hpp"
#include "../scene/World.h"

namespace gp {
class System {
 public:
  virtual void onConfigure(World& world) {}
  virtual void onPreUpdate(World& world) {}
  virtual void onUpdate(World& world, float delta_time) {}
  virtual void onPostUpdate(World& world) {}
  virtual void onRender(World& world) {}
  virtual void onExit(World& world) {}
};

}