#pragma once

#include <iterator>
#include <optional>

#include "../components/Transform.h"
#include "../ecs/Registry.hpp"
#include "Camera.h"
#include "GameObject.h"

namespace gp {

class World {
  template <typename... Components>
  struct View {
    View(ecs::View<Components...> view) : view_(view) {}

    template <typename Func>
    void each(Func func) {
      view_.each(func);
    }

   private:
    ecs::View<Components...> view_;
  };

  friend GameObject;

 public:
  // ----- CreateGameObject -----
  GameObject createGameObject();

  template <typename... Components>
  GameObject createGameObject(Components... comps) {
    GameObject gameObject = createGameObject();
    std::tuple<Components...> res =
        registry_.addComponent(gameObject.entity_, comps...);
    return gameObject;
  }

  // ------ DestroyGameObject -----
  bool destroyGameObject(GameObject& gameObject) {
    // If object has children, set their parent to parent of this (if it has
    // one)
    auto childrenItr = children_.find(gameObject.entity_);
    if (childrenItr != children_.end() && childrenItr->second.size() > 0) {
      auto newParentItr = parents_.find(gameObject.entity_);
      // If gameObject parent, set parent of gameObject children to this parent,
      // and add the children too
      if (newParentItr != parents_.end()) {
        for (auto childId : childrenItr->second) {
          parents_[childId] = newParentItr->second;
          children_[newParentItr->second].insert(childId);
        }
      } else {
        // No parent to assign, just clear these from parent/children now
        for (auto childId : childrenItr->second) {
          parents_.erase(childId);
        }
      }
    }

    // If gameObject has a parent, remove gameObject from parent children
    auto parentItr = parents_.find(gameObject.entity_);
    if (parentItr != parents_.end()) {
      children_[parentItr->second].erase(gameObject.entity_);
      parents_.erase(gameObject.entity_);
    }

    registry_.destroyEntity(gameObject.entity_);
    std::erase_if(gameObjects_,
                  [id = gameObject.entity_](const GameObject& obj) {
                    return obj.entity_ == id;
                  });
  }

  void setParent(GameObject& child, GameObject& parent) {
    auto parentIt = parents_.find(child.entity_);

    // If child already has a parent, and it's not the current parent
    // remove from list of children of existing parent
    if (parentIt != parents_.end() && parentIt->second != parent.entity_) {
      auto& oldChildren = children_[parentIt->second];
      assert(oldChildren.contains(child.entity_));
      oldChildren.erase(child.entity_);
    }

    // Now add to parents_ and children_
    parents_[child.entity_] = parent.entity_;
    children_[parent.entity_].insert(child.entity_);
  }

  std::optional<GameObject> getParent(const GameObject& gameObject) {
    if (parents_.contains(gameObject.entity_)) {
      return GameObject(parents_.at(gameObject.entity_), this);
    } else {
      return std::nullopt;
    }
  }

  std::vector<GameObject> getChildren(const GameObject& gameObject) {
    std::vector<GameObject> children;

    auto childrenItr = children_.find(gameObject.entity_);
    if (childrenItr != children_.end()) {
      std::transform(childrenItr->second.begin(), childrenItr->second.end(),
                     std::back_inserter(children),
                     [this](const ecs::Entity& entity) {
                       return GameObject(entity, this);
                     });
    }

    return children;
  }

  // ----- AddComponent -----
  template <typename T>
  T& addComponent(GameObject& gameObject) {
    return registry_.addComponent<T>(gameObject.entity_);
  }

  template <typename T>
  T& addComponent(GameObject& gameObject, T component) {
    return registry_.addComponent<T>(gameObject.entity_, component);
  }

  template <typename... Components, typename = typename std::enable_if<
                                        sizeof...(Components) >= 2>::type>
  std::tuple<Components&...> addComponents(GameObject& gameObject) {
    return registry_.addComponent<Components...>(gameObject.entity_);
  }

  template <typename... Components, typename = typename std::enable_if<
                                        sizeof...(Components) >= 2>::type>
  std::tuple<Components&...> addComponent(GameObject& gameObject,
                                          Components... comps) {
    return registry_.addComponent<Components...>(gameObject.entity_, comps);
  }

  // ----- RemoveGameObject -----
  template <typename T>
  void removeComponent(GameObject& gameObject) {
    registry_.removeComponent<T>(gameObject.entity_);
  }

  template <typename T>
  bool hasComponent(GameObject& gameObject) {
    return registry_.hasComponent<T>(gameObject.entity_);
  }

  // ----- View -----
  template <typename... Components>
  View<Components...> view() {
    return registry_.view<Components...>();
  }

  // ----- GetComponent ------
  template <typename T>
  T& getComponent(GameObject& gameObject) {
    return registry_.getComponent<T>(gameObject.entity_);
  }

  std::optional<GameObject> getGameObject(ecs::Entity ent) {
    auto gameObjectItr = std::find_if(gameObjects_.begin(), gameObjects_.end(),
                                      [ent](const GameObject& gameObject) {
                                        return gameObject.entity_ == ent;
                                      });

    if (gameObjectItr == gameObjects_.end()) return std::nullopt;
    return *gameObjectItr;
  }

  std::vector<GameObject>::const_iterator begin() {
    return gameObjects_.begin();
  }

  std::vector<GameObject>::const_iterator end() { return gameObjects_.end(); }

  Camera& getCamera() { return this->camera_; }
  void setCamera(Camera camera) { this->camera_ = std::move(camera); }

 private:
  std::vector<GameObject> gameObjects_;
  std::unordered_map<ecs::Entity, ecs::Entity> parents_;
  std::unordered_map<ecs::Entity, std::set<ecs::Entity>> children_;

  ecs::Registry registry_;

  Camera camera_;
};

}  // namespace gp