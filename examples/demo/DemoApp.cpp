#include "DemoApp.h"

#include <shaders/ShaderManager.h>

#include "components/RotateComponent.h"

void DemoApp::setup() {
  resource_manager_.loadTexture("textures\\metal_plate_diff_1k.png",
                                "metal_diff");
  resource_manager_.loadTexture("textures\\metal_plate_rough_1k.png",
                                "metal_rough");
  resource_manager_.loadTexture("textures\\metal_plate_nor_gl_1k.png",
                                "metal_norm");
  resource_manager_.loadTexture("textures\\metal_plate_metal_1k.png",
                                "metal_metal");

  MeshRenderer monkey_component =
      resource_manager_.loadObject("meshes\\monkey.obj", "monkey")[0];
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
  monkey_component.material_comp_ = metal_material;

  auto camera_ent = registry.createEntity();
  Camera& camera =
      registry.addComponent<Camera>(camera_ent, glm::vec3(-7.0f, 5.0f, -5.0f));

  auto cube_0 = registry.createEntity();
  registry.addComponent<Transform>(
      cube_0, Transform({0, -3, 0}, {0, 0, 0}, {10, 1, 10}));
  MeshRenderer& mr_0 =
      registry.addComponent<MeshRenderer>(cube_0, cube_component);

  auto monkey_ent = registry.createEntity();
  registry.addComponent<Transform>(
      monkey_ent,
      Transform(glm::vec3(0, 3.5, 0), glm::vec3(0), glm::vec3(2.0f)));
  registry.addComponent<MeshRenderer>(monkey_ent, monkey_component);

  auto light_0 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_0, glm::vec3(-2.5f, 6.0f, 1.0f),
                                        glm::vec3(0.4f, 0.4f, 1.0f), 8.0f);
  auto light_1 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_1, glm::vec3(2.5f, 6.0f, 0.0f),
                                        glm::vec3(1.0f, 1.0f, 1.0f), 15.0f);
  auto light_2 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_2, glm::vec3(0.0f, 6.0f, -2.5f),
                                        glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);

  MeshRenderer cube_mesh = mr_0;
  // cube_mesh.material_comp_ = metal_material;
  for (float i = 0; i < 5; i++) {
    for (float j = 0; j < 5; j++) {
      registry.createEntity<MeshRenderer, Transform, Rotate>(
          cube_mesh,
          Transform({-(i - 2) * 3, -2, -(j - 2) * 2.5}, {0, 0, 0},
                    {0.5, 0.5, 0.5}),
          {(i / 10), 0, j / 10});
    }
  }

  system_manager->addSystem(&camera_movement_system);
  system_manager->addSystem(&rotate_system);
}

void DemoApp::load() {}