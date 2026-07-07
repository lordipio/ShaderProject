#pragma once
#include "glm/gtc/type_ptr.hpp"
#include "Material.h"

typedef struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Mesh
{

public:
	Mesh();
	// ADD DRAW HERE LATER //

	std::vector<Vertex> vertices;
	std::vector<int> indices;

	Material* GetMaterial() { return material; };
	void SetMaterial(Material* newMaterial) { if (newMaterial) material = newMaterial; }

private:
	Material* material;
};