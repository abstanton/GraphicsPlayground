#pragma once
#include <string>
#include <unordered_map>

#include "../gpu/GPUBackend.h"
#include "../resources/core/Material.h"

using std::string;

enum class BuiltinShader {
  DEFAULT,
  DEPTH,
  EMISSIVE,
  PBR,
  POST,
  SHADOW,
};

class ShaderManager {
 public:
  static gpu::ShaderProgram* getShader(string name);
  static string getShaderName(BuiltinShader type);
  static Material getMaterialForBuiltin(BuiltinShader type);
  static bool addShaderProgram(const char* vs_filename, const char* fs_filename,
                               string shader_name);

 private:
  static ShaderManager& get() {
    static ShaderManager shader_manager = {};
    return shader_manager;
  }
  ShaderManager();

  gpu::ShaderProgram* compileFromFiles(const char* vs_filename,
                                       const char* fs_filename) const;

  gpu::ShaderProgram* ShaderManager::compileVertFragShaderProgram(
      const char* vs_source, const char* fs_source) const;

  std::unordered_map<string, gpu::ShaderProgram*> custom_shaders_;
  std::unordered_map<string, gpu::ShaderProgram*> builtin_shaders_;
};
