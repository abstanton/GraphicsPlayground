#pragma once
#include "System.h"
#include "../renderer/Renderer.h"

class RenderSystem : public System
{
public:
	RenderSystem() {}
	RenderSystem(Renderer* renderer_);

	void onConfigure(ecs::Registry& registry) override;
	void onRender(ecs::Registry& registry) override;
	void onExit(ecs::Registry& registry) override;
private:
	Renderer* renderer_;
	ecs::Entity camera_entity_;
};

