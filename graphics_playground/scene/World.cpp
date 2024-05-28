#include "World.h"

namespace gp {

GameObject World::createGameObject() {
  auto [entity, _] = registry_.createEntity(Transform());
  GameObject gameObject(entity, this);
  gameObjects_.push_back(gameObject);
  return gameObject;
}

}