#pragma once
#include <vector>

#include "System.h"

namespace gp {

class SystemManager {
 public:
  SystemManager(ecs::Registry& registry) : registry_(registry) {}

  void addSystem(System* system);
  void configure();
  void update(float deltaTime);
  void render();
  void exit();

 private:
  std::vector<System*> systems_;
  ecs::Registry& registry_;
};

}