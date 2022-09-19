#pragma once
#include <memory>
#include <typeinfo>

#include "ComponentArray.hpp"
#include "View.hpp"

namespace ecs {
class ComponentManager {
 public:
  template <typename T>
  void registerComponent() {
    const char* t_name = typeid(T).name();
    component_arrays_[t_name] = std::make_unique<ComponentArray<T>>();
  }

  template <typename T>
  T& addComponent(Entity entity, T component) {
    const char* t_name = typeid(T).name();
    assert(component_arrays_.find(t_name) != component_arrays_.end());
    return getComponentArray<T>().insertData(entity, component);
  }

  template <typename T>
  T& getComponent(Entity entity) {
    const char* t_name = typeid(T).name();
    assert(component_arrays_.find(t_name) != component_arrays_.end());
    return getComponentArray<T>().getData(entity);
  }

  template <typename T>
  void removeComponent(Entity entity) {
    const char* t_name = typeid(T).name();
    assert(component_arrays_.find(t_name) != component_arrays_.end());
    getComponentArray<T>()->deleteData(entity);
  }

  void entityDestroyed(Entity entity) {
    for (auto const& pair : component_arrays_) {
      pair.second->entityDestroyed(entity);
    }
  }

  template <typename... Components>
  View<Components...> view() {
    return View<Components...>(getComponentArray<Components>()...);
  }

  template <typename T>
  bool isComponentRegistered() {
    const char* t_name = typeid(T).name();
    return component_arrays_.find(t_name) != component_arrays_.end();
  }

 private:
  template <typename T>
  ComponentArray<T>& getComponentArray() {
    const char* t_name = typeid(T).name();
    if (component_arrays_.find(t_name) == component_arrays_.end()) {
      registerComponent<T>();
    }
    return *static_cast<ComponentArray<T>*>(component_arrays_[t_name].get());
  }

  std::unordered_map<const char*, std::unique_ptr<IComponentArray>>
      component_arrays_;
};
}  // namespace ecs
