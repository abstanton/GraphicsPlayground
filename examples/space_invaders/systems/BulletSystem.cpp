#include "BulletSystem.h"

#include <components/PointLight.h>
#include <components/Transform.h>

#include "../components/Bullet.h"
#include "iostream"

void BulletSystem::onUpdate(ecs::Registry& registry, float delta_time) {
  registry.view<Bullet, Transform, PointLight>().each([&](ecs::Entity ent,
                                                          Bullet& bullet,
                                                          Transform& transform,
                                                          PointLight& light) {
    transform.setPosition(transform.position() + bullet.velocity * delta_time);
    if (transform.position().y > 20.0f) {
      // TODO: Can I do this in a loop? not sure, i think I can cos it doesn't
      // move memory around afaik
      registry.destroyEntity(ent);
    }
  });
}