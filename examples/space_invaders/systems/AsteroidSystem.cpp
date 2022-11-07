#include "AsteroidSystem.h"

#include <components/Transform.h>

float f_rand() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float randomInRange(float low, float high) {
  return f_rand() * (high - low) + low;
}

void AsteroidSystem::onUpdate(ecs::Registry& registry, float delta_time) {
  // first delete asteroids too far away
  int num_alive_asteroids = 0;
  registry.view<Asteroid, Transform>().each(
      [&](ecs::Entity ent, Asteroid&, Transform& trans) {
        if ((trans.position().y < -8.0) || (abs(trans.position().x) > 10.0f)) {
          registry.destroyEntity(ent);
        } else {
          num_alive_asteroids++;
        }
      });

  // add needed asteroids
  for (int i = num_alive_asteroids; i < max_asteroids; i++) {
    // sample random position
    glm::vec3 position = {randomInRange(x_spawn_range.x, x_spawn_range.y),
                          randomInRange(y_spawn_range.x, y_spawn_range.y), 0};
    glm::vec3 velocity = {randomInRange(x_vel_range.x, x_vel_range.y),
                          randomInRange(y_vel_range.x, y_vel_range.y), 0};
    glm::vec3 rot_axis = {randomInRange(-1, 1), randomInRange(-1, 1),
                          randomInRange(-1, 1)};
    rot_axis = glm::normalize(rot_axis);
    float angular_vel = randomInRange(-max_ang_vel, max_ang_vel);

    float scale = randomInRange(scale_range.x, scale_range.y);

    std::uniform_int_distribution<int> distr(
        0, static_cast<int>(asteroid_meshes_.size()) - 1);
    int mesh_index = distr(gen);

    spawnAsteroid(registry, mesh_index, {velocity, rot_axis, angular_vel},
                  Transform(position, glm::vec3(0), glm::vec3(scale)));
  }

  // // upate asteroids by vel and angular vel
  registry.view<Asteroid, Transform>().each(
      [&](ecs::Entity ent, Asteroid& asteroid, Transform& trans) {
        trans.setPosition(trans.position() + asteroid.velocity * delta_time);
        // trans.setRotation(glm::rotate(trans.rotation(),
        //                               asteroid.ang_velocity * delta_time,
        //                               asteroid.rot_axis));
      });
}

void AsteroidSystem::spawnAsteroid(ecs::Registry& registry, int mesh_index,
                                   Asteroid asteroid, Transform transform) {
  auto& mesh_renderer = asteroid_meshes_.at(mesh_index);
  registry.createEntity<MeshRenderer, Asteroid, Transform>(mesh_renderer,
                                                           asteroid, transform);
}