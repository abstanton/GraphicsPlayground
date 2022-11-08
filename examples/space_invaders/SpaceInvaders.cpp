#include "SpaceInvaders.h"

#include <shaders/ShaderManager.h>

#include <iostream>

#include "components/Player.h"

void SpaceInvaders::setup() {
  renderer->setClearColour({0.0, 0.0, 0.0});

  std::vector<MeshRenderer> asteroid_meshes =
      ResourceManager::loadObject("meshes\\asteroids.fbx", "asteroids");
  ResourceManager::loadTexture("textures\\asteroid\\Mat1_Base_Color.jpeg",
                               "asteroid_diffuse");
  ResourceManager::loadTexture("textures\\asteroid\\Mat1_Metallic.jpeg",
                               "asteroid_metalness");
  ResourceManager::loadTexture("textures\\asteroid\\Mat1_Normal_DirectX.jpeg",
                               "asteroid_normal");
  ResourceManager::loadTexture("textures\\asteroid\\Mat1_Roughness.jpeg",
                               "asteroid_roughness");

  Material asteroid_material =
      ShaderManager::getMaterialForBuiltin(BuiltinShader::PBR);
  asteroid_material.setColourInput("diffuse", "asteroid_diffuse");
  asteroid_material.setColourInput("normal", "asteroid_normal");
  asteroid_material.setFloatInput("roughness", "asteroid_roughness");
  asteroid_material.setFloatInput("metalness", "asteroid_metalness");

  for (auto& asteroid : asteroid_meshes) {
    asteroid.material = asteroid_material;
  }

  MeshRenderer spaceship_mesh =
      ResourceManager::loadObject("meshes\\Spaceship.fbx", "spaceship")[0];
  ResourceManager::loadTexture("textures\\spaceship\\Osn_Base_color.jpg",
                               "spaceship_base");
  ResourceManager::loadTexture("textures\\spaceship\\Osn_Metallic.jpg",
                               "spaceship_metallic");
  ResourceManager::loadTexture("textures\\spaceship\\Osn_Normal_OpenGL.jpg",
                               "spaceship_normal");
  ResourceManager::loadTexture("textures\\spaceship\\Osn_Roughness.jpg",
                               "spaceship_roughness");
  Material spaceship_material =
      ShaderManager::getMaterialForBuiltin(BuiltinShader::PBR);
  spaceship_material.setColourInput("diffuse", "spaceship_base");
  spaceship_material.setColourInput("normal", "spaceship_normal");
  spaceship_material.setFloatInput("metalness", "spaceship_metallic");
  spaceship_material.setFloatInput("roughness", "spaceship_roughness");

  MeshRenderer bullet_mesh =
      ResourceManager::loadObject("meshes\\sphere.obj", "sphere")[0];
  Material bullet_material =
      ShaderManager::getMaterialForBuiltin(BuiltinShader::EMISSIVE);
  bullet_material.setColourInput("colour", glm::vec3(10.0f, 5.0f, 5.0f));
  bullet_mesh.material = bullet_material;

  spaceship_mesh.material = spaceship_material;

  auto camera_ent = registry.createEntity();
  Camera& camera =
      registry.addComponent<Camera>(camera_ent, glm::vec3(0.0f, 0.0f, 10.0f));
  camera.setExposure(1.0f);

  auto player_ent = registry.createEntity();
  registry.addComponent<Transform>(
      player_ent,
      Transform(glm::vec3(0, -2.0f, 0), glm::vec3(0), glm::vec3(0.1f)));
  registry.addComponent<MeshRenderer>(player_ent, spaceship_mesh);
  registry.addComponent<Player>(player_ent, Player());

  auto direction_light = registry.createEntity<DirectionLight>(
      DirectionLight({0, 1, 0.5}, {1.0, 1.0, 1.0}, 30.0f));

  player_controller_system =
      std::make_unique<PlayerControllerSystem>(bullet_mesh);
  system_manager->addSystem(player_controller_system.get());

  bullet_system = std::make_unique<BulletSystem>();
  system_manager->addSystem(bullet_system.get());

  asteroid_system = std::make_unique<AsteroidSystem>(asteroid_meshes);
  system_manager->addSystem(asteroid_system.get());
}

void SpaceInvaders::load() {}