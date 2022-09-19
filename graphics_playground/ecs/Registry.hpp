#pragma once
#include <set>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"

/**
 * This ECS is heavily taken from the tutorial found here:
 * https://austinmorlan.com/posts/entity_component_system/
 * with features from entt (https://github.com/skypjack/entt)
 * hacked on top.
 *
 */

namespace ecs {

class Registry {
 public:
  Registry() {}

  Entity createEntity() {
    Entity entity = entity_manager_.create();
    entities_.insert(entity);
    return entity;
  }

  template <typename... Components>
  std::tuple<Entity, Components&...> createEntity(Components... comps) {
    Entity entity = createEntity();
    std::tuple<Components&...> comps_tuple =
        addComponent<Components...>(entity, comps...);
    return std::tuple_cat(std::make_tuple(entity), comps_tuple);
  }

  void destroyEntity(Entity entity) {
    entity_manager_.destroyEntity(entity);
    component_manager_.entityDestroyed(entity);
    entities_.erase(entity);
  }

  template <typename T>
  T& addComponent(Entity entity) {
    if (!component_manager_.isComponentRegistered<T>()) {
      component_manager_.registerComponent<T>();
    }
    T component();
    return component_manager_.addComponent<T>(entity, component);
  }

  template <typename... Components, typename = typename std::enable_if<
                                        sizeof...(Components) >= 2>::type>
  std::tuple<Components&...> addComponent(Entity entity) {
    return std::forward_as_tuple(addComponent<Component>(entity)...);
  }

  template <typename T>
  T& addComponent(Entity entity, T component) {
    if (!component_manager_.isComponentRegistered<T>()) {
      component_manager_.registerComponent<T>();
    }
    return component_manager_.addComponent<T>(entity, component);
  }

  template <typename... Components, typename = typename std::enable_if<
                                        sizeof...(Components) >= 2>::type>
  std::tuple<Components&...> addComponent(Entity entity, Components... comps) {
    return std::forward_as_tuple(addComponent<Components>(entity, comps)...);
  }

  template <typename T, typename... Args>
  T& addComponent(Entity entity, Args&&... args) {
    if (!component_manager_.isComponentRegistered<T>()) {
      component_manager_.registerComponent<T>();
    }
    T component(std::forward<Args>(args)...);
    return component_manager_.addComponent<T>(entity, component);
  }

  template <typename T>
  void removeComponent(Entity entity) {
    component_manager_.removeComponent<T>(entity);
  }

  template <typename... Components>
  View<Components...> view() {
    return component_manager_.view<Components...>();
  }

  template <typename T>
  T& getComponent(Entity entity) {
    return component_manager_.getComponent<T>(entity);
  }

  std::set<Entity>::const_iterator begin() { return entities_.begin(); }

  std::set<Entity>::const_iterator end() { return entities_.end(); }

 private:
  ComponentManager component_manager_;
  EntityManager entity_manager_;

  std::set<Entity> entities_;
};
}  // namespace ecs
