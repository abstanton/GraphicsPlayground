#include "ShaderManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../../shaders/default.fs.h"
#include "../../shaders/default.vs.h"
#include "../../shaders/light.fs.h"
#include "../../shaders/pbr.fs.h"
#include "../../shaders/shadow.fs.h"
#include "../../shaders/shadow.vs.h"

ShaderManager::ShaderManager() {
  std::cout << "Loading builtin shaders" << std::endl;
  gpu::Backend* backend = gpu::Backend::get();
  builtin_shaders_["default"] = backend->compileShaderProgram(def_vs, def_fs);
  builtin_shaders_["emissive"] =
      backend->compileShaderProgram(def_vs, light_fs);
  builtin_shaders_["pbr"] = backend->compileShaderProgram(def_vs, pbr_fs);
  builtin_shaders_["shadow"] =
      backend->compileShaderProgram(shadow_vs, shadow_fs);
  std::cout << "Finished" << std::endl;
}

gpu::Shader* ShaderManager::getShader(string name) const {
  if (builtin_shaders_.find(name) != builtin_shaders_.end()) {
    return (*builtin_shaders_.find(name)).second;
  }
  if (custom_shaders_.find(name) != custom_shaders_.end()) {
    return (*custom_shaders_.find(name)).second;
  }
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

gpu::Shader* ShaderManager::compileFromFiles(const char* vs_filename,
                                             const char* fs_filename) const {
  gpu::Backend* backend = gpu::Backend::get();
  std::string vertex_code;
  std::string fragment_code;
  std::ifstream v_shader_file;
  std::ifstream f_shader_file;
  // ensure ifstream objects can throw exceptions:
  v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
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
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_LOADED\n" << std::endl;
    return nullptr;
  }

  return backend->compileShaderProgram(vertex_code.c_str(), def_fs);
}

bool ShaderManager::addShaderProgram(const char* vs_filename,
                                     const char* fs_filename, string name) {
  gpu::Shader* shader = compileFromFiles(vs_filename, fs_filename);
  if (shader != nullptr) {
    custom_shaders_[name] = shader;
    return true;
  }
  return false;
}

Material genDefaultMaterial() {
  MaterialBuilder builder;
  return builder.addColourInput("colour").setShaderName("default").build();
}
Material genEmissiveMaterial() { return Material{}; }
Material genPBRMaterial() {
  MaterialBuilder builder;
  return builder.addColourInput("diffuse")
      .addColourInput("normal")
      .addFloatInput("roughness")
      .addFloatInput("metalness")
      .setShaderName("pbr")
      .build();
}

Material ShaderManager::getMaterialForBuiltin(BuiltinShader type) const {
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
