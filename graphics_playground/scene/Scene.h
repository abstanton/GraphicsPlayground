#pragma once

#include "../components/Transform.h"
#include "../ecs/Registry.hpp"
#include "GameObject.h"

namespace gp {

class Scene {
 public:
  Scene() {}

  GameObject createObject() {
    top_level_objects.push_back(GameObject(registry_.createEntity()));
    return top_level_objects.back();
  }

  template <typename... Components>
  GameObject createEntity(Components... comps) {
    Entity entity = registry_.createEntity();
    std::tuple<Components&...> comps_tuple =
        registry_.addComponent<Components...>(entity, comps...);
    top_level_objects.push_back(GameObject(entity));
    return top_level_objects.back();
  }

  template <typename T>
  T& getComponent(const GameObject& object) {
    return registry_.getComponent<T>(object.entity_);
  }

  template <typename T>
  const T& getConstComponent(const GameObject& object) {
    return registry.getComponent<T>(object.entity_);
  }

  Transform& getComponent(const GameObject& object) {
    Transform& transform = registry.getComponent<Transform>(object.entity_);
    if (!object.transform_dirty) {
      return transform.world_transformation();
    }
    transform.setWorldTransformation(this->getWorldTransformation(object));
    object.transform_dirty = true;
    return transform;
  }

  const Transform& getConstComponent(const GameObject& object) {
    Transform& transform = registry.getComponent<Transform>(object.entity_);
    if (!object.transform_dirty) {
      return transform.world_transformation();
    }
    transform.setWorldTransformation(this->getWorldTransformation(object));
    object.transform_dirty = false;
    return transform;
  }

  template <typename... Components>
  View<Components...> view() {
    return registry_.view<Components...>();
  }

 private:
  ecs::Registry registry_;

  std::vector<GameObject> top_level_objects;
};

}