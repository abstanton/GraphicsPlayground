#pragma once

#include <systems/System.h>

using namespace gp;

class BulletSystem : public System {
 public:
  void onUpdate(ecs::Registry& registry, float delta_time) override;
};