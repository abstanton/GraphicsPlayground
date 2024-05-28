#include "SystemManager.h"

#include <algorithm>
namespace gp {

void SystemManager::addSystem(System* system) { systems_.push_back(system); }
void SystemManager::configure(World& world) {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onConfigure(world); });
}
void SystemManager::update(World& world, float delta_time) {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onPreUpdate(world); });
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onUpdate(world, delta_time); });
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onPostUpdate(world); });
}
void SystemManager::render(World& world) {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onRender(world); });
}
void SystemManager::exit(World& world) {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onExit(world); });
}

}