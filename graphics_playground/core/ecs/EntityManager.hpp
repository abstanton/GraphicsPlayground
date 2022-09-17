#pragma once
#include "Constants.hpp"
#include <queue>
#include <assert.h>

namespace ecs {
	class EntityManager
	{
	public:
		EntityManager() {
			for (Entity entity = 0; entity < MAX_ENTITIES; entity++) {
				available_entities_.push(entity);
			}
		}

		Entity create() {
			assert(available_entities_.size() > 0);
			Entity entity = available_entities_.front();
			available_entities_.pop();
			return entity;
		}

		void destroyEntity(Entity entity) {
			available_entities_.push(entity);
		}
	private:
		std::queue<Entity> available_entities_;
	};
}

