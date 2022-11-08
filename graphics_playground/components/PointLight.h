#pragma once
#include <glm/glm.hpp>

struct PointLight {
  float intensity;
  glm::vec3 colour;
  PointLight() {}
  PointLight(glm::vec3 colour, float intensity)
      : intensity(intensity), colour(colour) {}
};
