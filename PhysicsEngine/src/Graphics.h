#include <SDL.h>
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



typedef struct FaceIndices
{
	struct FaceIndex
	{
		glm::ivec3 vertexIndex;
	};

	FaceIndex faceIndices[3];
};

typedef struct Vertex
{
	glm::vec3 pos;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};

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
	std::string LoadShaderFile(std::string shaderPath);
	void CreateProgram(GLuint vertexShader, GLuint fragShader);
	GLuint CreateShader(std::string shaderPath, GLuint shaderType);
	GLuint program;

	GLuint VAO;
	GLuint VBO;
	GLuint IBO;

	glm::mat4x4 model;
	glm::mat4x4 view;
	glm::mat4x4 proj;

	void InitWindow();

	void CreateBuffers();

	void CreateVBO();
	void CreateVAO();
	void CreateIBO();

	std::vector<float> globalVertices = { -1,  1, 0, 0, 1, 1, 0, 0,
									 1,  1, 0, 1, 1, 0, 1, 0,
									 1, -1, 0, 1, 0, 0, 0, 1,
									-1, -1, 0, 0, 0, 0, 1, 1 };

	std::vector<int> indices = { 0, 3, 1,
								 1, 3, 2 };

	void PreDraw();
	void ClearScreen();
	void Draw();

	void ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height);
	void LoadTexture(const char* path, GLuint& textureID);

	GLuint baseTextureID;
	GLuint overlayTextureID;

	float currentTime;

	Mesh* LoadMesh(std::string path);
	std::vector<Mesh*> meshes;

};