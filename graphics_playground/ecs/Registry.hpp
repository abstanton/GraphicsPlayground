#pragma once
#include "ComponentManager.hpp"
#include "EntityManager.hpp"

#include <set>

namespace ecs {

	class Registry
	{
	public:
		Registry() {}

		Entity createEntity() {
			Entity entity = entity_manager_.create();
			entities_.insert(entity);
			return entity;
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

		template <typename T>
		T& addComponent(Entity entity, T component) {
			if (!component_manager_.isComponentRegistered<T>()) {
				component_manager_.registerComponent<T>();
			}
			return component_manager_.addComponent<T>(entity, component);
		}

		template <typename T, typename... Args>
		T& addComponent(Entity entity, Args &&...args) {
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

		std::set<Entity>::const_iterator begin() {
			return entities_.begin();
		}

		std::set<Entity>::const_iterator end() {
			return entities_.end();
		}

	private:
		ComponentManager component_manager_;
		EntityManager entity_manager_;

		std::set<Entity> entities_;
	};
}

