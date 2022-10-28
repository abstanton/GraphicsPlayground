#include "SystemManager.h"

#include <algorithm>

void SystemManager::addSystem(System* system) { systems_.push_back(system); }
void SystemManager::configure() {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onConfigure(registry_); });
}
void SystemManager::update(float delta_time) {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onPreUpdate(registry_); });
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onUpdate(registry_, delta_time); });
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onPostUpdate(registry_); });
}
void SystemManager::render() {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onRender(registry_); });
}
void SystemManager::exit() {
  std::for_each(systems_.begin(), systems_.end(),
                [&](System* sys) { sys->onExit(registry_); });
}