#include "PBRMaterial.h"

PBRMaterial::PBRMaterial(std::shared_ptr<gpu::Shader> shader, Texture* diffuse_texture, Texture* roughness_texture, Texture* normal_texture,  Texture* metalness_texture) : diffuse_texture_(diffuse_texture), roughness_texture_(roughness_texture), normal_texture_(normal_texture), metalness_texture_(metalness_texture){
	shader_ = shader;
}