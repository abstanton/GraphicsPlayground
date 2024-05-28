#pragma once

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
      return view_.each(func);
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
    registry_.destroyEntity(gameObject.entity_);
    std::erase_if(gameObjects_, [id = gameObject.id](const GameObject& obj) {
      return obj.id == id;
    });
    // TODO: sort out children and parents
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

  std::vector<GameObject>::const_iterator begin() {
    return gameObjects_.begin();
  }

  std::vector<GameObject>::const_iterator end() { return gameObjects_.end(); }

  Camera& getCamera() { return this->camera_; }
  void setCamera(Camera camera) { this->camera_ = std::move(camera); }

 private:
  std::vector<GameObject> gameObjects_;
  std::unordered_map<GameObject::ID, GameObject::ID> parents_;
  std::unordered_map<GameObject::ID, std::set<GameObject::ID>> children_;

  ecs::Registry registry_;

  Camera camera_;
};

}  // namespace gp