#pragma once

#include "../../renderer/Renderer.h"
#include "../System.h"

namespace gp {

class RenderSystem : public System {
 public:
  RenderSystem() {}
  RenderSystem(Renderer* renderer_);

  void onConfigure(World& world) override;
  void onRender(World& world) override;
  void onExit(World& world) override;

 private:
  Renderer* renderer_;
};

}