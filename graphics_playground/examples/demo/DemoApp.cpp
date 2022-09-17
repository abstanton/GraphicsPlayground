#include "DemoApp.h"

#include "../../core/managers/ShaderManager.h"
#include "components/RotateComponent.h"

void DemoApp::setup() {
  resource_manager_.loadTexture("asphalt_alb.png", "asphalt_diff");
  resource_manager_.loadTexture("asphalt_spec.png", "asphalt_rough");
  resource_manager_.loadTexture("asphalt_norm.png", "asphalt_norm");

  resource_manager_.loadTexture("metal_plate_diff_1k.png", "metal_diff");
  resource_manager_.loadTexture("metal_plate_rough_1k.png", "metal_rough");
  resource_manager_.loadTexture("metal_plate_nor_gl_1k.png", "metal_norm");
  resource_manager_.loadTexture("metal_plate_metal_1k.png", "metal_metal");

  MeshRenderer monkey_component =
      resource_manager_.loadObject("monkey.obj", "monkey")[0];
  MeshRenderer cube_component =
      resource_manager_.loadObject("cube.obj", "cube")[0];

  Material asphalt_material_comp =
      ShaderManager::get().getMaterialForBuiltin(BuiltinShader::PBR);
  asphalt_material_comp.setColourInput("diffuse", "asphalt_diff", {10, 10});
  asphalt_material_comp.setColourInput("normal", glm::vec3(0, 0, 1.0f));
  asphalt_material_comp.setFloatInput("roughness", 1.0);
  asphalt_material_comp.setFloatInput("metalness", 0);

  Material metal_material =
      ShaderManager::get().getMaterialForBuiltin(BuiltinShader::PBR);
  metal_material.setColourInput("diffuse", "metal_diff", {5, 5});
  metal_material.setColourInput("normal", "metal_norm", {5, 5});
  metal_material.setFloatInput("roughness", "metal_rough", {5, 5});
  metal_material.setFloatInput("metalness", "metal_metal", {5, 5});

  cube_component.material_comp_ = metal_material;

  MeshRenderer new_mesh_comp(monkey_component.mesh_, asphalt_material_comp,
                             monkey_component.transform_);
  monkey_component = new_mesh_comp;

  auto camera_ent = registry.createEntity();
  Camera& camera =
      registry.addComponent<Camera>(camera_ent, glm::vec3(0.0f, 0.0f, 3.0f));

  Transform transform;
  transform.position = glm::vec3(0, -3.0, 0);
  transform.scale = glm::vec3(10.0, 1.0, 10.0);
  cube_component.material_comp_ = metal_material;
  cube_component.transform_ = transform;
  auto cube_0 = registry.createEntity();
  MeshRenderer& mr_0 =
      registry.addComponent<MeshRenderer>(cube_0, cube_component);

  transform.position = glm::vec3(5, 0, 0);
  transform.euler.z = pi<float>() / 2;
  cube_component.transform_ = transform;
  auto cube_1 = registry.createEntity();
  MeshRenderer& mr_1 =
      registry.addComponent<MeshRenderer>(cube_1, cube_component);

  transform.position = glm::vec3(0, 0, 5);
  transform.euler.x = pi<float>() / 2;
  cube_component.transform_ = transform;
  auto cube_2 = registry.createEntity();
  registry.addComponent<MeshRenderer>(cube_2, cube_component);

  glm::vec3 cube_positions[] = {
      glm::vec3(0.0f, 0.0f, 5.0f),   glm::vec3(2.0f, 5.0f, -10.0f),
      glm::vec3(-1.5f, -2.2f, 2.5f), glm::vec3(-3.8f, -2.0f, -7.3f),
      glm::vec3(2.4f, -0.4f, 1.5f),  glm::vec3(-1.7f, 3.0f, -2.5f),
      glm::vec3(1.3f, -2.0f, 2.5f),  glm::vec3(1.5f, 2.0f, 2.5f),
      glm::vec3(1.5f, 0.2f, 3.5f),   glm::vec3(-1.3f, 1.0f, 3.5f)};

  monkey_component.material_comp_ = asphalt_material_comp;
  transform.position = glm::vec3(0.0, 0.5, 0.0);
  transform.scale = glm::vec3(1.2f);
  monkey_component.transform_ = transform;
  auto monkey_ent = registry.createEntity();
  registry.addComponent<MeshRenderer>(monkey_ent, monkey_component);
  Rotate rotate_comp = {20, 20, 20};
  registry.addComponent<Rotate>(monkey_ent, rotate_comp);

  auto light_0 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_0, glm::vec3(-2.5f, 6.0f, 1.0f),
                                        glm::vec3(0.4f, 0.4f, 1.0f), 8.0f);
  auto light_1 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_1, glm::vec3(2.5f, 6.0f, 0.0f),
                                        glm::vec3(1.0f, 1.0f, 1.0f), 15.0f);
  auto light_2 = registry.createEntity();
  registry.addComponent<DirectionLight>(light_2, glm::vec3(0.0f, 6.0f, -2.5f),
                                        glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);

  registry.view<Camera>().each([&](ecs::Entity ent, Camera& cam) {
    std::cout << "Camera: " << cam.Front.x << std::endl;
  });

  system_manager->addSystem(&camera_movement_system);
  system_manager->addSystem(&rotate_system);
}

void DemoApp::load() {}

/*
Material System Overhaul TODO:

 - Convert MaterialComp to store ordered map of name->TexOr and name->Uniform
 - Use ResourceHandle rather than raw pointers.
 - Add TextureHandle to texture inputs on materials.
 - Figure out how to get the texture binding dynamically.

 // These functions get the input, set the relevant 'use_tex' flag, fill the
required value material_comp.setInput(string name, glm::vec3 val);
 material_comp.setInput(string name, TextureHandle tex);

 - Make constructing a material easier, use MaterialBuilder?


 class MaterialBuilder {
 public:
        MaterialBuilder& addColourInput(string name) {
                material_.colourInputs[name] = {(1,1,1), false, {}}
                return *this;
        }

        MaterialComp build() {
                return material_;
        }
 private:
        MaterialComp material_;
 }

 MaterialBuilder b;
 b.addColourInput("name")
 b.addColourInput("other name")
 b.addFloatInput("specular")
 b.addFloatInput("metallic").build()

 */