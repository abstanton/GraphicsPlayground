#include "World.h"

namespace gp {

GameObject World::createGameObject() {
  auto [entity, _] = registry_.createEntity(Transform());
  GameObject gameObject(entity, this);
  gameObjects_.push_back(gameObject);
  return gameObject;
}

Transform GameObject::getLocalTransform() {
  return this->world_->getComponent<Transform>(*this);
}

// Walk hierarchy
glm::mat4 GameObject::getGlobalTransform() {
  auto parentOpt = this->world_->getParent(*this);
  if (!parentOpt.has_value()) {
    return getLocalTransform().transformation();
  }
  return getLocalTransform().transformation() * parentOpt->getGlobalTransform();
}

}  // namespace gp