#pragma once
#include <string>
#include <unordered_map>

#include "../../gpu/GPUBackend.h"
#include "../../resources/Material.h"

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
  static ShaderManager& get() {
    static ShaderManager shader_manager = {};
    return shader_manager;
  }

  gpu::Shader* getShader(string name) const;
  string getShaderName(BuiltinShader type) const;
  Material getMaterialForBuiltin(BuiltinShader type) const;
  bool addShaderProgram(const char* vs_filename, const char* fs_filename,
                        string shader_name);

 private:
  ShaderManager();

  gpu::Shader* compileFromFiles(const char* vs_filename,
                                const char* fs_filename) const;

  std::unordered_map<string, gpu::Shader*> custom_shaders_;
  std::unordered_map<string, gpu::Shader*> builtin_shaders_;
};
