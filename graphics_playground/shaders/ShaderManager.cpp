#include "ShaderManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "core/blur.fs.h"
#include "core/default.fs.h"
#include "core/default.vs.h"
#include "core/light.fs.h"
#include "core/pbr.fs.h"
#include "core/post.fs.h"
#include "core/quad.vs.h"
#include "core/shadow.fs.h"
#include "core/shadow.vs.h"
#include "core/ssao.fs.h"

ShaderManager::ShaderManager() {
  std::cout << "Loading builtin shaders" << std::endl;
  builtin_shaders_["blur"] =
      this->compileVertFragShaderProgram(quad_vs, blur_fs);
  builtin_shaders_["default"] =
      this->compileVertFragShaderProgram(def_vs, def_fs);
  builtin_shaders_["depth"] =
      this->compileVertFragShaderProgram(def_vs, shadow_fs);
  builtin_shaders_["emissive"] =
      this->compileVertFragShaderProgram(def_vs, light_fs);
  builtin_shaders_["pbr"] = this->compileVertFragShaderProgram(def_vs, pbr_fs);
  builtin_shaders_["post"] =
      this->compileVertFragShaderProgram(quad_vs, post_fs);
  builtin_shaders_["ssao"] =
      this->compileVertFragShaderProgram(quad_vs, ssao_fs);
  builtin_shaders_["shadow"] =
      this->compileVertFragShaderProgram(shadow_vs, shadow_fs);
  std::cout << "Finished" << std::endl;
}

gpu::ShaderProgram* ShaderManager::compileVertFragShaderProgram(
    const char* vs_source, const char* fs_source) const {
  gpu::ShaderProgram* shader_program =
      gpu::Backend::get()->allocShaderProgram();
  shader_program->addShaderFromSource(gpu::ShaderType::VERT, vs_source);
  shader_program->addShaderFromSource(gpu::ShaderType::FRAG, fs_source);
  shader_program->link();
  return shader_program;
}

gpu::ShaderProgram* ShaderManager::getShader(string name) {
  auto& inst = ShaderManager::get();
  if (inst.builtin_shaders_.find(name) != inst.builtin_shaders_.end()) {
    return (*inst.builtin_shaders_.find(name)).second;
  }
  if (inst.custom_shaders_.find(name) != inst.custom_shaders_.end()) {
    return (*inst.custom_shaders_.find(name)).second;
  }
  return nullptr;
}

string ShaderManager::getShaderName(BuiltinShader type) {
  switch (type) {
    case BuiltinShader::DEFAULT:
      return "default";
    case BuiltinShader::DEPTH:
      return "depth";
    case BuiltinShader::EMISSIVE:
      return "emissive";
    case BuiltinShader::PBR:
      return "pbr";
    case BuiltinShader::POST:
      return "post";
    case BuiltinShader::SHADOW:
      return "shadow";
    default:
      return "default";
  }
}

gpu::ShaderProgram* ShaderManager::compileFromFiles(
    const char* vs_filename, const char* fs_filename) const {
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

  return compileVertFragShaderProgram(vertex_code.c_str(), def_fs);
}

bool ShaderManager::addShaderProgram(const char* vs_filename,
                                     const char* fs_filename, string name) {
  auto& inst = ShaderManager::get();
  gpu::ShaderProgram* shader = inst.compileFromFiles(vs_filename, fs_filename);
  if (shader != nullptr) {
    inst.custom_shaders_[name] = shader;
    return true;
  }
  return false;
}

Material genDefaultMaterial() {
  MaterialBuilder builder;
  return builder.addColourInput("colour").setShaderName("default").build();
}
Material genEmissiveMaterial() {
  MaterialBuilder builder;
  return builder.addColourInput("colour").setShaderName("emissive").build();
}

Material genPBRMaterial() {
  MaterialBuilder builder;
  return builder.addColourInput("diffuse")
      .addColourInput("normal")
      .addFloatInput("roughness")
      .addFloatInput("metalness")
      .setShaderName("pbr")
      .build();
}

Material ShaderManager::getMaterialForBuiltin(BuiltinShader type) {
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
