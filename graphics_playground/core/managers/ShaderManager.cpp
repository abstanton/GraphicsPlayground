#include "ShaderManager.h"
#include <fstream>
#include <sstream>
#include <iostream>


ShaderManager::ShaderManager() {
	/* Load all default shaders. */
	/* TODO: clean this up. */
	builtin_shaders_["default"] = compileFromFiles("shaders/defaultVS.vs", "shaders/defaultFS.fs");
	builtin_shaders_["emissive"] = compileFromFiles("shaders/defaultVS.vs", "shaders/lightFS.fs");
	builtin_shaders_["pbr"] = compileFromFiles("shaders/defaultVS.vs", "shaders/pbrFS.fs");
	builtin_shaders_["shadow"] = compileFromFiles("shaders/shadowVS.vs", "shaders/shadowFS.fs");
}

gpu::Shader* ShaderManager::getShader(string name) const {
	if (builtin_shaders_.find(name) != builtin_shaders_.end()) {
		std::cout << "Found Builtin " << name << std::endl;
		return (*builtin_shaders_.find(name)).second;
	}
	if (custom_shaders_.find(name) != custom_shaders_.end()) {
		std::cout << "Found Custom " << name << std::endl;
		return (*custom_shaders_.find(name)).second;
	}
	std::cout << "Could not find " << name << std::endl;

	return nullptr;
}

string ShaderManager::getShaderName(BuiltinShader type) const {
	switch (type) {
	case BuiltinShader::DEFAULT:
		return "default";
	case BuiltinShader::EMISSIVE:
		return "emissive";
	case BuiltinShader::PBR:
		return "pbr";
	case BuiltinShader::SHADOW:
		return "shadow";
	default:
		return "default";
	}
}

gpu::Shader* ShaderManager::compileFromFiles(const char* vs_filename, const char* fs_filename) const {
	gpu::Backend* backend = gpu::Backend::get();
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	// ensure ifstream objects can throw exceptions:
	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		v_shader_file.open(vs_filename);
		f_shader_file.open(fs_filename);
		std::stringstream v_shader_stream, f_shader_stream;
		// read file's buffer contents into streams
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();
		// close file handlers
		v_shader_file.close();
		f_shader_file.close();
		// convert stream into string
		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_LOADED\n" << std::endl;
		return nullptr;
	}
	return backend->compileShaderProgram(vertex_code.c_str(), fragment_code.c_str());
}

bool ShaderManager::addShaderProgram(const char* vs_filename, const char* fs_filename, string name) {
	
	gpu::Shader* shader = compileFromFiles(vs_filename, fs_filename);
	if (shader != nullptr) {
		custom_shaders_[name] = shader;
		return true;
	}
	return false;
}

MaterialComp genDefaultMaterial() {
	return MaterialComp{};
}
MaterialComp genEmissiveMaterial() {
	return MaterialComp{};
}
MaterialComp genPBRMaterial() {
	MaterialBuilder builder;
	return builder
		.addColourInput("diffuse")
		.addColourInput("normal")
		.addFloatInput("roughness")
		.addFloatInput("metalness")
		.build();
}

MaterialComp ShaderManager::getMaterialForBuiltin(BuiltinShader type) const {
	switch (type) {
	case BuiltinShader::DEFAULT:
		return genDefaultMaterial();
	case BuiltinShader::EMISSIVE:
		return genEmissiveMaterial();
	case BuiltinShader::PBR:
		return genPBRMaterial();
	default:
		return genDefaultMaterial();
	} 
}
