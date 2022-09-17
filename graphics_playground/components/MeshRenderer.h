#pragma once
#include "../DefaultMaterial.h"
#include "../Material.h"
#include "../Transform.h"
#include "../resources/Mesh.h"

/*
 * MeshRenderer is an mesh that can be rendered.
 * Has a mesh, material and transform.
 */
// struct MeshRenderer
// {
// public:
// 	Mesh* mesh_;
// 	std::shared_ptr<Material> material_;
// 	MaterialComp material_comp_;
// 	//TODO Remove this
// 	Transform transform_;
// 	bool cast_shadows = true;

// 	MeshRenderer() {};
// 	MeshRenderer(Mesh* mesh, std::shared_ptr<Material> material) :
// mesh_(mesh), material_(material) {} 	MeshRenderer(Mesh* mesh,
// std::shared_ptr<Material> material, Transform transform): mesh_(mesh),
// material_(material), transform_(transform) {} 	MeshRenderer(Mesh* mesh,
// std::shared_ptr<Material> material, MaterialComp mat_comp, Transform
// transform): mesh_(mesh), material_(material), material_comp_(mat_comp),
// transform_(transform) {}

// };

struct MeshRenderer {
 public:
  Mesh* mesh_;
  std::shared_ptr<Material> material_;
  MaterialComp material_comp_;
  // TODO Remove this
  Transform transform_;
  bool cast_shadows = true;

  MeshRenderer(){};
  MeshRenderer(Mesh* mesh, std::shared_ptr<Material> material)
      : mesh_(mesh), material_(material) {}
  MeshRenderer(Mesh* mesh, std::shared_ptr<Material> material,
               Transform transform)
      : mesh_(mesh), material_(material), transform_(transform) {}
  MeshRenderer(Mesh* mesh, std::shared_ptr<Material> material,
               MaterialComp mat_comp, Transform transform)
      : mesh_(mesh),
        material_(material),
        material_comp_(mat_comp),
        transform_(transform) {}
};
