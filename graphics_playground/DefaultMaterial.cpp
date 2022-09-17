#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial(std::shared_ptr<gpu::Shader> shader, glm::vec3 colour) : colour_(colour) {
	shader_ = shader;
}

//void DefaultMaterial::use() const {
//	shader_->use();
//	shader_->setVec3("colour", colour_);
//}