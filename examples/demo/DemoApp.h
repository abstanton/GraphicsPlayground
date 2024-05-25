#pragma once
#include <GraphicsPlayground.h>

#include "systems/CameraMovementSystem.h"
#include "systems/RotateSystem.h"

class DemoApp : public gp::GraphicsPlayground {
 public:
  void setup() override;
  void load() override;

 private:
  CameraMovementSystem camera_movement_system;
  RotateSystem rotate_system;
};
