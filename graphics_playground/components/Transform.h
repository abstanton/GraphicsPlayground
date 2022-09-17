#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform {
  glm::vec3 position;
  glm::vec3 euler;
  glm::vec3 scale;
  glm::fquat rot;

  // TODO: Refactor this to store quarternion, gets updated on setRotation,
  // Position, Scale etc
  glm::mat4 getTransformation() {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotation = glm::toMat4(glm::quat(euler));
    glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);

    return translation * rotation * scale_mat;
  }

  void setRotation(float x, float y, float z) {
    rot = glm::fquat(glm::vec3(x, y, z));
  }

  Transform() {
    position = glm::vec3();
    euler = glm::vec3();
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
  }

  Transform(glm::vec3 pos, glm::vec3 eul, glm::vec3 sca)
      : position(pos), euler(eul), scale(sca) {}
};