#pragma once

#include "../../renderer/Renderer.h"
#include "../System.h"

namespace gp {

class RenderSystem : public System {
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

}