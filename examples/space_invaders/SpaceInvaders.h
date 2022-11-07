#pragma once
#include <GraphicsPlayground.h>

#include "systems/AsteroidSystem.h"
#include "systems/BulletSystem.h"
#include "systems/PlayerControllerSystem.h"

class SpaceInvaders : public GraphicsPlayground {
 public:
  void setup() override;
  void load() override;

 private:
  std::unique_ptr<PlayerControllerSystem> player_controller_system;
  std::unique_ptr<BulletSystem> bullet_system;
  std::unique_ptr<AsteroidSystem> asteroid_system;
};
