#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <map>
#include <optional>

#include "../../IResource.h"
#include "../../components/MeshRenderer.h"
#include "../../resources/Mesh.h"
#include "../../resources/Texture.h"
#include "../UUIDGenerator.h"

using std::map;
using std::string;
using std::vector;

class ResourceManager {
 public:
  /*
   * Load texture resource, return loaded resource pointer
   */
  std::optional<Texture*> loadTextureResource(const string& path);
  /*
   * Load Mesh resource, return loaded resource pointer
   */
  std::optional<Mesh*> loadMeshResource(const string& path);

  bool loadTexture(const char* path, const char* name);
  bool loadMesh(const char* path, const char* name);
  bool loadObject(const char* path, const char* base_name);
  /*
   * Load Object resource, consisting of Mesh and Textures
   */
  std::optional<MeshRenderer> loadObjectResource(const string& path);
  bool loadTextResource(const string& path);

 private:
  vector<std::shared_ptr<IResource>> loaded_resources_;

  map<string, Texture> loaded_textures_;
  map<string, Mesh> loaded_meshes_;

  Assimp::Importer importer_;
  std::shared_ptr<DefaultMaterial> default_material_;

  void processNode(const aiScene* scene, aiNode* node,
                   std::vector<MeshRenderer>& meshes, const char* base_name);
  Mesh processMesh(const char* base_name, aiMesh* mesh);
};
