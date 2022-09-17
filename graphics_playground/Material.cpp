#include "Material.h"

void Material::setModelMatrix(const glm::mat4 model) const {
	shader_->setMat4("model", model);
}
