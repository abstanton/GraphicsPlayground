#include "PlayerControllerSystem.h"

// gp
#include <components/PointLight.h>
#include <components/Transform.h>
#include <platform/Input.h>

using namespace gp;

template <typename T>
T lerp(T curr, T target, float rate) {
  return curr + ((target - curr) * rate);
}

void PlayerControllerSystem::onUpdate(ecs::Registry& registry,
                                      float delta_time) {
  ecs::Entity player_entity;
  registry.view<Player>().each(
      [&](ecs::Entity ent, Player& player) { player_entity = ent; });

  Player& player = registry.getComponent<Player>(player_entity);
  Transform& player_transform = registry.getComponent<Transform>(player_entity);

  // Movement
  auto a_state = Input::getKeyState(Key::A);
  auto d_state = Input::getKeyState(Key::D);
  bool stopped = (a_state == KeyState::UP) && (d_state == KeyState::UP);

  if (stopped) {
    player.acceleration = {0, 0, 0};
    player.velocity = lerp(player.velocity, {0, 0, 0}, 0.1f);
  } else {
    player.acceleration.x =
        ((a_state == KeyState::DOWN)
             ? -player.accel_rate
             : ((d_state == KeyState::DOWN) ? player.accel_rate : 0)) *
        delta_time;
    player.velocity = glm::clamp(player.velocity + player.acceleration,
                                 -player.velocity_max, player.velocity_max);
  }
  player_transform.setPosition(player_transform.position() +
                               player.velocity * delta_time);

  float rot =
      ((a_state == KeyState::DOWN) ? -0.4f
                                   : ((d_state == KeyState::DOWN) ? 0.4f : 0));
  player_transform.setRotation(
      lerp<glm::vec3>(player_transform.euler(), {0, rot, 0}, 0.1f));

  // Shooting
  player.bullet_delta_time += delta_time;
  auto space_state = Input::getKeyState(Key::SPACE);
  if (space_state == KeyState::DOWN) {
    if (player.bullet_delta_time > 0.2f) {
      // Shoot bullet
      spawnBullet({1.0f, player.velocity + glm::vec3(0.0f, 25.0f, 0.0f)},
                  player_transform.position(), registry);
      player.bullet_delta_time = 0;
    }
  }
}

void PlayerControllerSystem::spawnBullet(Bullet bullet, glm::vec3 pos,
                                         ecs::Registry& registry) {
  registry.createEntity<Bullet, Transform, MeshRenderer, PointLight>(
      bullet, Transform(pos, {0, 0, 0}, {0.1, 0.1, 0.1}), bullet_mesh_,
      PointLight({1.0, 0.4, 0.4}, 20.0f));
}