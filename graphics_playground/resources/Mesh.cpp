#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices) : verts_(verts), indices_(indices) {
    has_indices_ = true;
}