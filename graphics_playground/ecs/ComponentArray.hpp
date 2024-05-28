#pragma once
#include <assert.h>

#include <array>
#include <unordered_map>

#include "Constants.hpp"

namespace ecs {

template <typename A>
struct SingleComponentIterator;

class IComponentArray {
 public:
  virtual void entityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
  friend SingleComponentIterator<T>;

 public:
  ComponentArray() : components_{} {}

  T& getData(Entity entity) {
    assert(entity_index_map_.find(entity) != entity_index_map_.end());
    size_t index = entity_index_map_[entity];
    assert(index < MAX_ENTITIES);
    return components_[index];
  }

  const T& getData(Entity entity) const {
    assert(entity_index_map_.find(entity) != entity_index_map_.end());
    size_t index = entity_index_map_.at(entity);
    assert(index < MAX_ENTITIES);
    return components_[index];
  }

  std::tuple<T&> getDataAsTuple(Entity entity) {
    return std::forward_as_tuple(getData(entity));
  }

  T& insertData(Entity entity, T component) {
    assert(curr_size_ + 1 < MAX_ENTITIES);
    if (entity_index_map_.find(entity) != entity_index_map_.end()) {
      components_[entity_index_map_[entity]] = component;
    } else {
      components_[curr_size_] = component;
      entity_index_map_[entity] = curr_size_;
      index_entity_map_[curr_size_] = entity;
      curr_size_++;
    }
    return components_[entity_index_map_[entity]];
  }

  void deleteData(Entity entity) {
    assert(entity_index_map_.find(entity) != entity_index_map_.end());

    // get index of entity component
    size_t index = entity_index_map_[entity];

    // get the last component in comp array to copy to removed entity
    size_t last_elem_index = curr_size_ - 1;
    Entity last_component_entity = index_entity_map_[last_elem_index];

    // overwrite comp to destroy with last component
    T last_component = components_[last_elem_index];
    components_[index] = last_component;

    // update index pointer to point to entity who owns the last comp
    index_entity_map_[index] = last_component_entity;
    entity_index_map_[last_component_entity] = index;

    entity_index_map_.erase(entity);
    index_entity_map_.erase(last_elem_index);
    curr_size_--;
  }

  void entityDestroyed(Entity entity) override {
    if (entity_index_map_.find(entity) != entity_index_map_.end()) {
      deleteData(entity);
    }
  }

  size_t size() { return curr_size_; }

  bool contains(Entity ent) {
    return entity_index_map_.find(ent) != entity_index_map_.end();
  }

  SingleComponentIterator<T> each() { return SingleComponentIterator<T>(this); }

 private:
  std::array<T, MAX_ENTITIES> components_{};
  std::unordered_map<Entity, size_t> entity_index_map_;
  std::unordered_map<size_t, Entity> index_entity_map_;
  size_t curr_size_ = 0;
};

template <typename T>
struct SingleComponentIterator {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = std::tuple<Entity, T>;
  using pointer = std::tuple<Entity, T*>;  // or also value_type*
  using reference = std::tuple<Entity, T&>;

  SingleComponentIterator(ComponentArray<T>* arr) : base_array(arr) {}
  SingleComponentIterator(ComponentArray<T>* arr, size_t index)
      : base_array(arr), curr_index(index) {}

  reference operator*() const {
    return {base_array->index_entity_map_[curr_index],
            base_array->components_[curr_index]};
  }
  pointer operator->() {
    return {base_array->index_entity_map_[curr_index],
            &base_array->components_[curr_index]};
  }

  SingleComponentIterator& operator++() {
    curr_index++;
    return *this;
  }
  SingleComponentIterator* operator++(int) {
    SingleComponentIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const SingleComponentIterator& a,
                         const SingleComponentIterator& b) {
    return a.curr_index == b.curr_index;
  }
  friend bool operator!=(const SingleComponentIterator& a,
                         const SingleComponentIterator& b) {
    return a.curr_index != b.curr_index;
  }

 public:
  SingleComponentIterator begin() {
    return SingleComponentIterator(base_array);
  }
  SingleComponentIterator end() {
    return SingleComponentIterator(base_array, base_array->curr_size_);
  }

 private:
  size_t curr_index = 0;
  ComponentArray<T>* base_array;
};
}  // namespace ecs
