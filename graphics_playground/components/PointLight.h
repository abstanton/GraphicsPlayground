#pragma once
#include<glm/glm.hpp>

struct PointLight
{
	float intensity;
	glm::vec3 position;
	glm::vec3 colour;
	PointLight(glm::vec3 position, glm::vec3 colour, float intensity) : intensity(intensity), position(position), colour(colour) {}
};

