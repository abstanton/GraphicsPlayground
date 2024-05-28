#pragma once
#include <vector>

#include "../scene/World.h"
#include "System.h"

namespace gp {

class SystemManager {
 public:
  SystemManager() {}

  void addSystem(System* system);
  void configure(World& world);
  void update(World& world, float deltaTime);
  void render(World& world);
  void exit(World& world);

 private:
  std::vector<System*> systems_;
};

}  // namespace gp