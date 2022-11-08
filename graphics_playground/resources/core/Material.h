#pragma once
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../components/Transform.h"
#include "../gpu/GPUShaderProgram.h"

// TODO: Move this, material is not a resource, it's an asset?

/*
        Represents the following glsl:
        uniform bool name_use_tex;
        uniform T name_val;
        uniform sampler2D name_tex;
        uniform vec2 name_scale;

*/
using std::string;

template <typename T>
struct TextureOr {
  T value;
  bool use_tex = false;
  glm::vec2 scale;
  string tex_name;

  void setValue(T val) {
    use_tex = false;
    value = val;
  }

  void setValue(string tex, glm::vec2 tex_scale = {1, 1}) {
    use_tex = true;
    tex_name = tex;
    scale = tex_scale;
  }

  TextureOr() {}
  TextureOr(T val) : value(val) {}
};

/*
        Represents the following glsl:
        uniform T name;
*/
template <typename T>
struct Uniform {
  T value;

  void setValue(T val) { value = val; }
};

struct Material {
  void setColourInput(string name, glm::vec3 value) {
    if (colour_inputs.find(name) == colour_inputs.end()) {
      return;
    }
    colour_inputs[name].setValue(value);
  }

  void setColourInput(string name, string texture_name,
                      glm::vec2 tex_scale = {1, 1}) {
    if (colour_inputs.find(name) == colour_inputs.end()) {
      return;
    }
    colour_inputs[name].setValue(texture_name, tex_scale);
  }

  void setFloatInput(string name, float value) {
    if (float_inputs.find(name) == float_inputs.end()) {
      return;
    }
    float_inputs[name].setValue(value);
  }

  void setFloatInput(string name, string texture_name,
                     glm::vec2 tex_scale = {1, 1}) {
    if (float_inputs.find(name) == float_inputs.end()) {
      return;
    }
    float_inputs[name].setValue(texture_name, tex_scale);
  }

  // TODO: encapsulate these
  std::map<string, TextureOr<glm::vec3>> colour_inputs;
  std::map<string, TextureOr<float>> float_inputs;

  std::map<string, Uniform<glm::vec3>> colour_uniforms;
  std::map<string, Uniform<float>> float_uniforms;
  std::map<string, Uniform<bool>> bool_uniforms;

  std::string shader_name;
  bool transparent;
  bool receive_shadows;
  bool cast_ao;
};

class MaterialBuilder {
 public:
  MaterialBuilder() {}

  MaterialBuilder& addColourInput(string name) {
    material_.colour_inputs[name] = TextureOr<glm::vec3>({1, 1, 1});
    return *this;
  }
  MaterialBuilder& addFloatInput(string name) {
    material_.float_inputs[name] = TextureOr<float>(1.0f);
    return *this;
  }
  MaterialBuilder& addColourUniform(string name) {
    material_.colour_uniforms[name] = Uniform<glm::vec3>{};
    return *this;
  }
  MaterialBuilder& addFloatUniform(string name) {
    material_.float_uniforms[name] = Uniform<float>{};
    return *this;
  }
  MaterialBuilder& addBoolUniform(string name) {
    material_.bool_uniforms[name] = Uniform<bool>{};
    return *this;
  }
  MaterialBuilder& setReceiveShadow(bool val) {
    material_.receive_shadows = val;
    return *this;
  }
  MaterialBuilder& setCastAO(bool val) {
    material_.cast_ao = val;
    return *this;
  }
  MaterialBuilder& setShaderName(string name) {
    material_.shader_name = name;
    return *this;
  }

  Material build() { return material_; }

 private:
  Material material_;
};
