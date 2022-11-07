#pragma once
#include <glm/glm.hpp>

struct Player {
  glm::vec3 velocity = {0, 0, 0};

  int health = 10;
  float damage = 2.0f;
  float movement_speed = 10.0f;

  float bullet_delta_time = 100000.0f;

  Player() {}
};