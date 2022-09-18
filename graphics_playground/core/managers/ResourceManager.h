#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <optional>

#include "../../components/MeshRenderer.h"
#include "../../resources/IResource.h"
#include "../../resources/Mesh.h"
#include "../../resources/Texture.h"
#include "../UUIDGenerator.h"

using std::map;
using std::string;
using std::vector;

class ResourceManager {
 public:
  static ResourceManager& get() {
    static ResourceManager resource_manager = {};
    return resource_manager;
  }

  /*
   * Load Mesh resource, return loaded resource pointer
   */
  std::optional<Mesh*> loadMeshResource(const string& path);

  bool loadTexture(const char* path, const char* name);
  bool loadMesh(const char* path, const char* name);
  std::vector<MeshRenderer> loadObject(const char* path, const char* base_name);

  std::optional<Texture*> getTexture(string tex_name);
  std::optional<Mesh*> getMesh(string mesh_name);
  /*
   * Load Object resource, consisting of Mesh and Textures
   */
  bool loadTextResource(const string& path);

 private:
  ResourceManager() {}

  vector<std::shared_ptr<IResource>> loaded_resources_;

  map<string, Texture> loaded_textures_;
  map<string, Mesh> loaded_meshes_;

  Assimp::Importer importer_;

  void processNode(const aiScene* scene, aiNode* node,
                   std::vector<MeshRenderer>& meshes, const char* base_name);
  Mesh processMesh(aiMesh* mesh);
};
