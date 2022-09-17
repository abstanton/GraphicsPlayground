#include "EmissiveMaterial.h"


EmissiveMaterial::EmissiveMaterial(std::shared_ptr<gpu::Shader> shader, glm::vec3 colour) : colour_(colour) {
	shader_ = shader;
}

//void EmissiveMaterial::use() const {
//	shader_->use();
//	shader_->setVec3("colour", colour_);
//}