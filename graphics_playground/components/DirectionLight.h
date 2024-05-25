#pragma once
#include <glm/glm.hpp>

namespace gp {

using namespace glm;

struct DirectionLight {
  vec3 direction;
  vec3 colour;
  float intensity;
  float size;

  DirectionLight() {}
  DirectionLight(vec3 dir, vec3 col, float siz, float inten)
      : direction(dir), colour(col), size(siz), intensity(inten) {}
};

}