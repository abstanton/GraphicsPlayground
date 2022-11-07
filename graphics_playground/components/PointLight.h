#pragma once
#include <glm/glm.hpp>

struct PointLight {
  float intensity;

  // TODO: refactor position to transform
  glm::vec3 position;
  glm::vec3 colour;
  PointLight() {}
  PointLight(glm::vec3 position, glm::vec3 colour, float intensity)
      : intensity(intensity), position(position), colour(colour) {}
};
