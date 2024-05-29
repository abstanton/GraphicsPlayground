#include "DemoApp.h"

#include <shaders/ShaderManager.h>

#include "components/RotateComponent.h"

using namespace gp;

void DemoApp::setup() {
  renderer->setClearColour({10.0, 10.0, 10.0});
  renderer->setAmbientLight({1.0, 1.0, 1.0});

  ResourceManager::loadTexture("textures\\metal_plate_diff_1k.png",
                               "metal_diff");
  ResourceManager::loadTexture("textures\\metal_plate_rough_1k.png",
                               "metal_rough");
  ResourceManager::loadTexture("textures\\metal_plate_nor_gl_1k.png",
                               "metal_norm");
  ResourceManager::loadTexture("textures\\metal_plate_metal_1k.png",
                               "metal_metal");

  MeshRenderer monkey_component =
      ResourceManager::loadObject("meshes\\monkey.obj", "monkey")[0];
  MeshRenderer cube_component =
      ResourceManager::loadObject("meshes\\cube.obj", "cube")[0];

  Material metal_material =
      ShaderManager::getMaterialForBuiltin(BuiltinShader::PBR);
  metal_material.setColourInput("diffuse", "metal_diff", {1, 1});
  metal_material.setColourInput("normal", "metal_norm", {1, 1});
  metal_material.setFloatInput("roughness", "metal_rough", {1, 1});
  metal_material.setFloatInput("metalness", "metal_metal", {1, 1});

  Material blank_material =
      ShaderManager::getMaterialForBuiltin(BuiltinShader::PBR);
  // metal_material = blank_material;

  cube_component.material = blank_material;
  monkey_component.material = metal_material;

  world.setCamera(Camera(glm::vec3(-7.0f, 5.0f, -5.0f)));

  auto cube_0 = world.createGameObject();
  world.addComponent<Transform>(cube_0,
                                Transform({0, -5, 0}, {0, 0, 0}, {10, 1, 10}));
  MeshRenderer& mr_0 = world.addComponent<MeshRenderer>(cube_0, cube_component);

  auto monkey_ent = world.createGameObject();
  world.addComponent<Transform>(
      monkey_ent,
      Transform(glm::vec3(0, 3.5, 0), glm::vec3(0), glm::vec3(2.0f)));
  world.addComponent<MeshRenderer>(monkey_ent, monkey_component);

  auto light_0 = world.createGameObject();
  world.addComponent<DirectionLight>(
      light_0, DirectionLight(-glm::vec3(-2.5f, 6.0f, 1.0f),
                              glm::vec3(0.4f, 0.4f, 1.0f), 1.0f, 2.0f));
  auto light_1 = world.createGameObject();
  world.addComponent<DirectionLight>(
      light_1, DirectionLight(-glm::vec3(2.5f, 6.0f, 0.0f),
                              glm::vec3(1.0f, 1.0f, 1.0f), 20.0f, 7.0f));
  auto light_2 = world.createGameObject();
  world.addComponent<DirectionLight>(
      light_2, DirectionLight(-glm::vec3(0.0f, 6.0f, -2.5f),
                              glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f));

  auto baseObject = world.createGameObject();

  MeshRenderer cube_mesh = mr_0;
  for (float i = 0; i < 5; i++) {
    for (float j = 0; j < 5; j++) {
      auto obj = world.createGameObject<MeshRenderer, Transform, Rotate>(
          cube_mesh,
          Transform({-(i - 2) * 3, -2, -(j - 2) * 2.5}, {0, 0, 0},
                    {0.5, 0.5, 0.5}),
          {(i / 10), 0, j / 10});
      world.setParent(obj, baseObject);
    }
  }

  Transform& transform = baseObject.getComponent<Transform>();
  transform.setPosition({0.0f, 0.0f, 0.0f});
  world.addComponent<Rotate>(baseObject, {0.3f, 0.0f, 0.5f});

  system_manager->addSystem(&camera_movement_system);
  system_manager->addSystem(&rotate_system);
}

void DemoApp::load() {}