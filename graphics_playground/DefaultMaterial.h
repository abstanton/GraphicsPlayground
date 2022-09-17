#pragma once
#include "Material.h"
#include <glm/glm.hpp>
class DefaultMaterial : public Material
{
public:
	DefaultMaterial(std::shared_ptr<gpu::Shader> shader, glm::vec3 colour);
	std::vector<const Texture*> getTextures() const override { return std::vector<const Texture*>{}; }
	glm::vec3 colour_;

};

