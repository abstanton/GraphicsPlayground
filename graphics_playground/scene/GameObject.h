#pragma once
#include <vector>

#include "../UUIDGenerator.h"
#include "../components/Transform.h"
#include "../ecs/Registry.hpp"

namespace gp {

class World;

// GameObjects are entities with transforms.
// They can only be created, moved, updated or edited in any way by the
// Scene which contains the GameObject
class GameObject {
  using ID = unsigned int;
  friend World;

  std::string getIdentifier() const { return identifier_; }
  void setIdentifier(std::string_view identifier) { identifier_ = identifier; }

  const World* getWorld() const { return world_; }
  // Transform getLocalTransform() {
  //   return world_->getComponent<Transform>(*this);
  // }

 private:
  GameObject(ecs::Entity entity, World* world)
      : entity_(entity), world_(world) {
    id = UUIDGenerator::getUUID();
  }
  ecs::Entity entity_;
  std::string identifier_;

  World* world_;
  ID id;
};

}  // namespace gp