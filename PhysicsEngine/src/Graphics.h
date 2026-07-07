#include <SDL.h>
#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include "glm/vec4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cstdlib>
#include <fstream>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080



//typedef struct FaceIndices
//{
//	struct FaceIndex
//	{
//		glm::ivec3 vertexIndex;
//	};
//
//	FaceIndex faceIndices[3];
//};
//
//typedef struct Vertex
//{
//	glm::vec3 pos;
//	glm::vec2 uv;
//	glm::vec3 normal;
//};
//
//struct Mesh
//{
//	std::vector<Vertex> vertices;
//	std::vector<int> indices;
//	Shader* shader;
//};
class Mesh;
class Graphics
{
public:
	Graphics();
	~Graphics();

	void Render();
	void Update(float dt);

private:
	void Setup();

	SDL_Window* window;
	SDL_GLContext glContext;

	void SetMVP();

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	glm::mat4x4 model;
	glm::mat4x4 model2;
	glm::mat4x4 view;
	glm::mat4x4 proj;

	void InitWindow();

	void CreateBuffers();

	void CreateVBO();
	void CreateVAO();
	void CreateIBO();

	void PreDraw();
	void ClearScreen();
	void Draw();

	float currentTime;

	Mesh* LoadMesh(std::string path);

	std::vector<Mesh*> meshes;

	Mesh* integrated_mesh;


};