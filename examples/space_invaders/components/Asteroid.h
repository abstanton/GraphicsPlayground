#pragma once
#include <glm/glm.hpp>

struct Asteroid {
  glm::vec3 velocity;
  glm::vec3 rot_axis;
  float ang_velocity;
};