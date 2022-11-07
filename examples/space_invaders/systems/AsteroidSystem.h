#pragma once

#include <components/MeshRenderer.h>
#include <systems/System.h>

#include <glm/glm.hpp>
#include <random>
#include <vector>

#include "../components/Asteroid.h"

class AsteroidSystem : public System {
 public:
  AsteroidSystem(std::vector<MeshRenderer> asteroid_meshes)
      : asteroid_meshes_(asteroid_meshes), gen(dev()) {}

  void onUpdate(ecs::Registry& registry, float delta_time) override;

 private:
  std::vector<MeshRenderer> asteroid_meshes_;

  int max_asteroids = 15;
  glm::vec2 x_spawn_range = {-5, 5};
  glm::vec2 y_spawn_range = {20.0f, 30.0f};
  glm::vec2 y_vel_range = {-4.0f, -8.0f};
  glm::vec2 x_vel_range = {-2.0f, 2.0f};
  float max_ang_vel = 1.0f;
  glm::vec2 scale_range = {0.1, 0.3};

  std::random_device dev;
  std::mt19937 gen;

  void spawnAsteroid(ecs::Registry& registry, int mesh_index, Asteroid asteroid,
                     Transform transform);
};