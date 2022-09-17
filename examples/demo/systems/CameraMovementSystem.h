#pragma once
#include <systems/System.h>

class CameraMovementSystem : public System {
 public:
  CameraMovementSystem() {}

  void onConfigure(ecs::Registry& registry) override;
  void onUpdate(ecs::Registry& registry, float delta_time) override;

 private:
  ecs::Entity camera_entity;
};