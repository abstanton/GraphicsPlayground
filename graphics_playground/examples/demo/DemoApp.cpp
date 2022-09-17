#include "DemoApp.h"

#include "../../DefaultMaterial.h"
#include "../../EmissiveMaterial.h"
#include "../../PBRMaterial.h"
#include "../../core/managers/ShaderManager.h"
#include "components/RotateComponent.h"

void DemoApp::setup() {
  std::shared_ptr<gpu::Shader> pbr_shader = std::make_shared<gpu::GlShader>(
      gpu::GlShader::fromFilepaths("shaders/defaultVS.vs", "shaders/pbrFS.fs")
          .value());
  std::shared_ptr<gpu::Shader> light_shader = std::make_shared<gpu::GlShader>(
      gpu::GlShader::fromFilepaths("shaders/defaultVS.vs", "shaders/lightFS.fs")
          .value());
  std::shared_ptr<gpu::Shader> default_shader = std::make_shared<gpu::GlShader>(
      gpu::GlShader::fromFilepaths("shaders/defaultVS.vs",
                                   "shaders/defaultFS.fs")
          .value());

  ShaderManager& shader_manager = ShaderManager::get();

  pbr_shader.reset(shader_manager.getShader("pbr"));

  std::shared_ptr<DefaultMaterial> default_material =
      std::make_shared<DefaultMaterial>(default_shader,
                                        glm::vec3(0.2, 1.0, 1.0));

  Texture* asphalt_alb_texture =
      resource_manager_.loadTextureResource("asphalt_alb.png").value();
  Texture* asphalt_roughness_texture =
      resource_manager_.loadTextureResource("asphalt_spec.png").value();
  Texture* asphalt_normal_texture =
      resource_manager_.loadTextureResource("asphalt_norm.png").value();

  Texture* metal_plate_alb =
      resource_manager_.loadTextureResource("metal_plate_diff_1k.png").value();
  Texture* metal_plate_rough =
      resource_manager_.loadTextureResource("metal_plate_rough_1k.png").value();
  Texture* metal_plate_norm =
      resource_manager_.loadTextureResource("metal_plate_nor_gl_1k.png")
          .value();
  Texture* metal_plate_metal =
      resource_manager_.loadTextureResource("metal_plate_metal_1k.png").value();

  std::shared_ptr<Material> metal_plate_material =
      std::make_shared<PBRMaterial>(pbr_shader, metal_plate_alb,
                                    metal_plate_rough, metal_plate_norm,
                                    metal_plate_metal);
  metal_plate_material->tex_scale = 5.0f;

  MeshRenderer monkey_component =
      resource_manager_.loadObjectResource("monkey.obj").value();
  MeshRenderer cube_component =
      resource_manager_.loadObjectResource("cube.obj").value();

  std::cout << "Monkey ID: " << monkey_component.mesh_->id << std::endl;
  std::cout << "Cube ID: " << cube_component.mesh_->id << std::endl;

  MaterialComp pbr_material =
      shader_manager.getMaterialForBuiltin(BuiltinShader::PBR);

  std::shared_ptr<PBRMaterial> asphalt_material = std::make_shared<PBRMaterial>(
      pbr_shader, asphalt_alb_texture, asphalt_roughness_texture,
      asphalt_normal_texture, asphalt_roughness_texture);
  std::shared_ptr<PBRMaterial> asphalt_material_norm =
      std::make_shared<PBRMaterial>(
          pbr_shader, asphalt_normal_texture, asphalt_roughness_texture,
          asphalt_normal_texture, asphalt_roughness_texture);
  std::shared_ptr<EmissiveMaterial> emissive_material =
      std::make_shared<EmissiveMaterial>(light_shader,
                                         glm::vec3(1.0, 1.0, 1.0));

  auto camera_ent = registry.createEntity();
  Camera& camera =
      registry.addComponent<Camera>(camera_ent, glm::vec3(0.0f, 0.0f, 3.0f));

  Transform transform;
  transform.position = glm::vec3(0, -3.0, 0);
  transform.scale = glm::vec3(10.0, 1.0, 10.0);
  cube_component.material_ = metal_plate_material;
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

  monkey_component.material_ = asphalt_material;
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