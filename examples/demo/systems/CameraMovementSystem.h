#pragma once
#include <systems/System.h>

class CameraMovementSystem : public gp::System {
 public:
  CameraMovementSystem() {}

  void onUpdate(gp::World& world, float delta_time) override;
};