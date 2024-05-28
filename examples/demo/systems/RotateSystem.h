#pragma once
#include <systems/System.h>

class RotateSystem : public gp::System {
  void onUpdate(gp::World& world, float delta_time) override;
};
