#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <optional>

#include "../UUIDGenerator.h"
#include "../components/MeshRenderer.h"
#include "core/IResource.h"
#include "core/Mesh.h"
#include "core/Texture.h"

using std::map;
using std::string;
using std::vector;

/*
TODO:

Return ResourceHandle<Type> instead of raw pointer
then, we have a

template <typename T>
struct ResourceHandle {
  string name;
  uint64 id;
}

Type* resource_ptr = ResourceManager::getResource<Type>(ResourceHandle<Type>
handle);

ResourceHandle<Type> handle = ResourceManager::getHandle(string name);

ResourceHandle<Type> handle = ResourceManager::loadResource<Type>(string path,
options etc)

Then,
*/

class ResourceManager {
 public:
  /*
   * Load Mesh resource, return loaded resource pointer
   */
  static std::optional<Mesh*> loadMeshResource(const string& path);

  static bool loadTexture(const char* path, const char* name);
  static bool loadMesh(const char* path, const char* name);
  static std::vector<MeshRenderer> loadObject(const char* path,
                                              const char* base_name);

  static std::optional<Texture*> getTexture(string tex_name);
  static std::optional<Mesh*> getMesh(string mesh_name);
  /*
   * Load Object resource, consisting of Mesh and Textures
   */
  static bool loadTextResource(const string& path);

 private:
  ResourceManager() {}

  static ResourceManager& get() {
    static ResourceManager resource_manager = {};
    return resource_manager;
  }

  vector<std::shared_ptr<IResource>> loaded_resources_;

  map<string, Texture> loaded_textures_;
  map<string, Mesh> loaded_meshes_;

  Assimp::Importer importer_;

  void processNode(const aiScene* scene, aiNode* node,
                   std::vector<MeshRenderer>& meshes, const char* base_name);
  Mesh processMesh(aiMesh* mesh);
};
