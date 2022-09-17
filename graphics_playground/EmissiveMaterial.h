#pragma once
#include "Material.h"
#include <glm/glm.hpp>
class EmissiveMaterial : public Material
{
public:
	EmissiveMaterial(std::shared_ptr<gpu::Shader> shader, glm::vec3 colour);
	std::vector<const Texture*> getTextures() const override { return {}; }
	glm::vec3 colour_;
};

