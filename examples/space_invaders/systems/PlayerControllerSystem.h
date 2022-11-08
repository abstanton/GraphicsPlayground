#pragma once
#include <systems/System.h>

// space_invaders
#include "../components/Bullet.h"
#include "../components/Player.h"

// gp
#include <components/MeshRenderer.h>

class PlayerControllerSystem : public System {
 public:
  PlayerControllerSystem(MeshRenderer bullet_mesh)
      : bullet_mesh_(bullet_mesh) {}

  void onUpdate(ecs::Registry& registry, float delta_time) override;

 private:
  void spawnBullet(Bullet bullet, glm::vec3 pos, ecs::Registry& registry);
  MeshRenderer bullet_mesh_;
};