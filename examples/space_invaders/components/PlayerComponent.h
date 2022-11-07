#pragma once
#include <glm/glm.hpp>

struct Player {
  glm::vec3 velocity = {0, 0, 0};
  glm::vec3 acceleration = {0, 0, 0};
  glm::vec3 velocity_max = {10.0f, 10.0f, 10.0f};

  int health = 10;
  float damage = 2.0f;
  float accel_rate = 20.0f;

  float bullet_delta_time = 100000.0f;

  Player() {}
};