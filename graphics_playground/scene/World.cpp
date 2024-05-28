#include "World.h"

namespace gp {

GameObject World::createGameObject() {
  auto [entity, _] = registry_.createEntity(Transform());
  GameObject gameObject(entity, this);
  gameObjects_.push_back(gameObject);
  return gameObject;
}

Transform GameObject::getLocalTransform() const {
  return this->world_->getComponent<Transform>(*this);
}

// Walk hierarchy
Transform GameObject::getGlobalTransform() const {
  auto parentOpt = this->world_->getParent(*this);
  if (!parentOpt.has_value()) {
    return getLocalTransform();
  }
  return Transform(parentOpt->getGlobalTransform().transformation() *
                   getLocalTransform().transformation());
}

}  // namespace gp