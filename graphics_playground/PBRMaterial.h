#pragma once
#include "Material.h"

class PBRMaterial : public Material
{
public:
	Texture* diffuse_texture_;
	Texture* roughness_texture_;
	Texture* metalness_texture_;
	Texture* normal_texture_;

	std::vector<const Texture*> getTextures() const override { return std::vector<const Texture*>{ diffuse_texture_, roughness_texture_, normal_texture_, roughness_texture_ }; }

	PBRMaterial(std::shared_ptr<gpu::Shader> shader, Texture* diffuse_texture, Texture* roughness_texture, Texture* normal_texture, Texture* metalness_texture);
};

