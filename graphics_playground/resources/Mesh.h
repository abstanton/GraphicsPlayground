#pragma once
#include "../IResource.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec3 tang;
	glm::vec2 uv;
};

struct Mesh : IResource
{
public:
	std::vector<Vertex> verts_;
	std::vector<unsigned int> indices_;


	int num_verts_;
	bool has_indices_;

	Mesh() : has_indices_(false), num_verts_(0) {}
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
};

