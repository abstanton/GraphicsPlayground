#include "PlayerControllerSystem.h"

// gp
#include <components/PointLight.h>
#include <components/Transform.h>
#include <platform/Input.h>

void PlayerControllerSystem::onUpdate(ecs::Registry& registry,
                                      float delta_time) {
  ecs::Entity player_entity;
  registry.view<Player>().each(
      [&](ecs::Entity ent, Player& player) { player_entity = ent; });

  Player& player = registry.getComponent<Player>(player_entity);
  Transform& player_transform = registry.getComponent<Transform>(player_entity);

  // Shooting
  player.bullet_delta_time += delta_time;
  auto space_state = Input::get().getKeyState(Key::SPACE);
  if (space_state == KeyState::DOWN) {
    if (player.bullet_delta_time > 0.2f) {
      // Shoot bullet
      spawnBullet({1.0f, glm::vec3(0.0f, 20.0f, 0.0f)},
                  player_transform.position(), registry);
      player.bullet_delta_time = 0;
    }
  }

  // Movement
  auto a_state = Input::get().getKeyState(Key::A);
  auto d_state = Input::get().getKeyState(Key::D);

  player.velocity.x =
      ((a_state == KeyState::DOWN)
           ? -player.movement_speed
           : ((d_state == KeyState::DOWN) ? player.movement_speed : 0)) *
      delta_time;

  player_transform.setPosition(player_transform.position() + player.velocity);
}

void PlayerControllerSystem::spawnBullet(Bullet bullet, glm::vec3 pos,
                                         ecs::Registry& registry) {
  registry.createEntity<Bullet, Transform, MeshRenderer, PointLight>(
      bullet, Transform(pos, {0, 0, 0}, {0.1, 0.1, 0.1}), bullet_mesh_,
      PointLight(pos, {1.0, 0.4, 0.4}, 20.0f));
}