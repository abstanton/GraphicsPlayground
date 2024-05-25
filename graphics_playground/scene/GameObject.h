#pragma once
#include <vector>

#include "../ecs/Registry.hpp"

namespace gp {

class Scene;

// GameObjects can only be created, moved, updated or edited in any way by the
// Scene which contains the GameObject
class GameObject {
  friend Scene;

 public:
  GameObject(ecs::Entity entity) : entity_(entity) {}

  GameObject getParent() const { return parent; }
  std::vector<GameObject> getChildren() { return children; }

 private:
  GameObject parent;
  std::vector<GameObject> children;
  ecs::Entity entity_;
  bool transform_dirty = true;
};

}