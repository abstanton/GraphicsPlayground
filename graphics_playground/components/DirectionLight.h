#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct DirectionLight {
  vec3 direction;
  vec3 colour;
  float intensity;

  DirectionLight() {}
  DirectionLight(vec3 dir, vec3 col, float inten)
      : direction(dir), colour(col), intensity(inten) {}
};