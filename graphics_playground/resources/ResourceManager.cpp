#include "ResourceManager.h"

#include <iostream>

#include "../shaders/ShaderManager.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

bool ResourceManager::loadTexture(const char* filepath, const char* name) {
  auto& inst = ResourceManager::get();

  int width, height, channels;
  unsigned char* data = stbi_load(filepath, &width, &height, &channels, 0);
  if (data) {
    unsigned int id = UUIDGenerator::getUUID();
    TextureFormat format;
    // TODO: handle all channels
    switch (channels) {
      case 1:
        format = TextureFormat::R;
        break;
      case 2:
        format = TextureFormat::RG8;
      case 3:
        format = TextureFormat::RGB8;
        break;
      case 4:
        format = TextureFormat::RGBA8;
        break;
      default:
        format = TextureFormat::RGB8;
    }

    Texture texture(data, width, height, format);
    texture.path = filepath;
    texture.id = id;
    inst.loaded_textures_.emplace(name, texture);
    return true;
  } else {
    std::cout << "Failed to load texture" << std::endl;
    return false;
  }
}

std::optional<Texture*> ResourceManager::getTexture(string name) {
  auto& inst = ResourceManager::get();
  if (inst.loaded_textures_.find(name) == inst.loaded_textures_.end()) {
    return {};
  }
  return &inst.loaded_textures_[name];
}

std::optional<Mesh*> ResourceManager::getMesh(string name) {
  auto& inst = ResourceManager::get();
  if (inst.loaded_meshes_.find(name) == inst.loaded_meshes_.end()) {
    return {};
  }
  return &inst.loaded_meshes_[name];
}

static glm::vec3 assimpToGlmVec3(aiVector3D assimp_vec) {
  return glm::vec3(assimp_vec.x, assimp_vec.y, assimp_vec.z);
}

static glm::vec2 assimpToGlmVec2(aiVector2D assimp_vec) {
  return glm::vec2(assimp_vec.x, assimp_vec.y);
}

void ResourceManager::processNode(const aiScene* scene, aiNode* node,
                                  std::vector<MeshRenderer>& mesh_renderers_out,
                                  const char* base_name) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    Mesh mesh = processMesh(scene->mMeshes[node->mMeshes[i]]);
    mesh.id = UUIDGenerator::getUUID();
    mesh.path = scene->mMeshes[node->mMeshes[i]]->mName.C_Str();

    std::string mesh_name =
        string(base_name) + std::to_string(mesh_renderers_out.size());

    loaded_meshes_.emplace(mesh_name, mesh);

    std::shared_ptr<Mesh> mesh_ptr = std::make_shared<Mesh>(mesh);
    loaded_resources_.push_back(mesh_ptr);
    Transform transform;
    aiVector3D scale;
    aiVector3D position;
    aiVector3D rotation;
    node->mTransformation.Decompose(scale, rotation, position);
    transform.setPosition(assimpToGlmVec3(position));
    transform.setScale(assimpToGlmVec3(scale));
    transform.setRotation(assimpToGlmVec3(rotation));
    // Refactor to return entities so that they can have transforms
    mesh_renderers_out.push_back(MeshRenderer(
        mesh_name,
        ShaderManager::getMaterialForBuiltin(BuiltinShader::DEFAULT)));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(scene, node->mChildren[i], mesh_renderers_out, base_name);
  }
}

Mesh ResourceManager::processMesh(aiMesh* mesh) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  bool has_tex_coords = mesh->mTextureCoords[0] != nullptr;
  if (!has_tex_coords) {
    std::cout << "No tex coords! " << std::endl;
  }
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    glm::vec2 tex_coords = has_tex_coords
                               ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                           mesh->mTextureCoords[0][i].y)
                               : glm::vec2(0.0f);
    vertices.push_back({assimpToGlmVec3(mesh->mVertices[i]),
                        assimpToGlmVec3(mesh->mNormals[i]),
                        assimpToGlmVec3(mesh->mTangents[i]), tex_coords});
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return Mesh(vertices, indices);
}

// TODO return SceneNode or equivalent (capable of having children)
std::vector<MeshRenderer> ResourceManager::loadObject(const char* filepath,
                                                      const char* base_name) {
  auto& inst = ResourceManager::get();
  const aiScene* scene = inst.importer_.ReadFile(
      filepath,
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (scene == nullptr) {
    return {};
  }
  std::vector<MeshRenderer> mesh_renderers;
  inst.processNode(scene, scene->mRootNode, mesh_renderers, base_name);

  return mesh_renderers;
}