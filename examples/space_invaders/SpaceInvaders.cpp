#include "SpaceInvaders.h"

#include <shaders/ShaderManager.h>

#include "components/PlayerComponent.h"

void SpaceInvaders::setup() {
  renderer->setClearColour({0, 0, 0});

  resource_manager_.loadTexture("textures\\metal_plate_diff_1k.png",
                                "metal_diff");
  resource_manager_.loadTexture("textures\\metal_plate_rough_1k.png",
                                "metal_rough");
  resource_manager_.loadTexture("textures\\metal_plate_nor_gl_1k.png",
                                "metal_norm");
  resource_manager_.loadTexture("textures\\metal_plate_metal_1k.png",
                                "metal_metal");

  MeshRenderer spaceship_mesh =
      resource_manager_.loadObject("meshes\\Spaceship.fbx", "spaceship")[0];
  resource_manager_.loadTexture("textures\\Osn_Base_color.jpg",
                                "spaceship_base");
  resource_manager_.loadTexture("textures\\Osn_Metallic.jpg",
                                "spaceship_metallic");
  resource_manager_.loadTexture("textures\\Osn_Normal_OpenGL.jpg",
                                "spaceship_normal");
  resource_manager_.loadTexture("textures\\Osn_Roughness.jpg",
                                "spaceship_roughness");

  Material spaceship_material =
      ShaderManager::get().getMaterialForBuiltin(BuiltinShader::PBR);
  spaceship_material.setColourInput("diffuse", "spaceship_base");
  spaceship_material.setColourInput("normal", "spaceship_normal");
  spaceship_material.setFloatInput("metalness", "spaceship_metallic");
  spaceship_material.setFloatInput("roughness", "spaceship_roughness");

  MeshRenderer cube_component =
      resource_manager_.loadObject("meshes\\cube.obj", "cube")[0];
  Material metal_material =
      ShaderManager::get().getMaterialForBuiltin(BuiltinShader::PBR);
  metal_material.setColourInput("diffuse", "metal_diff", {1, 1});
  metal_material.setColourInput("normal", "metal_norm", {1, 1});
  metal_material.setFloatInput("roughness", "metal_rough", {1, 1});
  metal_material.setFloatInput("metalness", "metal_metal", {1, 1});

  Material blank_material =
      ShaderManager::get().getMaterialForBuiltin(BuiltinShader::PBR);
  // metal_material = blank_material;

  cube_component.material_comp_ = blank_material;

  spaceship_mesh.material_comp_ = spaceship_material;

  auto camera_ent = registry.createEntity();
  Camera& camera =
      registry.addComponent<Camera>(camera_ent, glm::vec3(0.0f, 0.0f, 10.0f));

  auto player_ent = registry.createEntity();
  registry.addComponent<Transform>(
      player_ent,
      Transform(glm::vec3(0, -2.0f, 0), glm::vec3(0), glm::vec3(0.1f)));
  registry.addComponent<MeshRenderer>(player_ent, spaceship_mesh);
  registry.addComponent<Player>(player_ent, Player());

  auto direction_light = registry.createEntity<DirectionLight>(
      DirectionLight({0, 1, 0.5}, {1.0, 1.0, 1.0}, 10.0f));

  player_controller_system =
      std::make_unique<PlayerControllerSystem>(cube_component);
  system_manager->addSystem(player_controller_system.get());

  bullet_system = std::make_unique<BulletSystem>();
  system_manager->addSystem(bullet_system.get());
}

void SpaceInvaders::load() {}