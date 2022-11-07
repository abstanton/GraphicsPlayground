#pragma once

#include <systems/System.h>

class BulletSystem : public System {
 public:
  void onUpdate(ecs::Registry& registry, float delta_time) override;
};