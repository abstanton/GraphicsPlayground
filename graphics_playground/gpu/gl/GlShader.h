#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>
#include "../GPUShader.h"


namespace gpu {
	class GlShader : public Shader
	{
	public:
		static std::optional<GlShader> fromFilepaths(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		GlShader(const char* vertexSource, const char* fragmentSource);
		void use() const;
		void setBool(const std::string& name, const bool value) const override;
		void setInt(const std::string& name, const int value) const override;
		void setFloat(const std::string& name, const float value) const override;
		void setVec2(const std::string& name, const glm::vec2& value) const override;
		void setVec3(const std::string& name, const glm::vec3& value) const override;
		void setMat3(const std::string& name, const glm::mat3& value) const override;
		void setMat4(const std::string& name, const glm::mat4& value) const override;

		unsigned int shaderProgram_ = 0;
	};
}
