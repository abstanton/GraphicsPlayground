#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
  Transform() {
    pos = glm::vec3();
    rot = glm::quat({0, 0, 0});
    sca = glm::vec3(1.0f, 1.0f, 1.0f);
  }

  Transform(glm::vec3 pos, glm::vec3 eul, glm::vec3 sca)
      : pos(pos), rot(eul), sca(sca) {}

  void setPosition(glm::vec3 p) {
    pos = p;
    dirty = true;
  }

  void setRotation(glm::vec3 e) {
    rot = glm::fquat(e);
    dirty = true;
  }

  void setRotation(glm::fquat q) {
    rot = q;
    dirty = true;
  }

  void setScale(glm::vec3 s) {
    sca = s;
    dirty = true;
  }

  glm::vec3 position() const { return pos; }

  glm::vec3 euler() const { return glm::eulerAngles(rot); }

  glm::vec2 scale() const { return sca; }

  glm::quat rotation() const { return rot; }

  glm::mat4 transformation() const {
 
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
    glm::mat4 rotation = glm::toMat4(rot);
    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), sca);

    return translation * rotation * scale_mat;
  }

 private:
  bool dirty = true;

  glm::vec3 pos;
  glm::vec3 sca;
  glm::quat rot;
  glm::mat4 matrix_;
};