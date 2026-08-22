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


class Mesh;
class Graphics
{
public:
	Graphics();
	~Graphics();

	void Render();
	void Update(float dt);
	
	void CameraMoveForward();
	void CameraMoveBackward();
	void CameraMoveLeft();
	void CameraMoveRight();
	void CameraMoveUp();
	void CameraMoveDown();

	void CameraAddRotation(glm::vec2 direction);

	void ProcessInput(const Uint8* keyboardState, float dt);

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
	glm::mat4x4 skyboxView;
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

	glm::vec3 cameraPos = glm::vec3(0.f);
	glm::vec3 lookDirection = glm::vec3(0.f, 0.f, -1.f);
	float cameraMovementSpeed = 10.f;
	float cameraRotationSpeed = 0.2f;
	float deltaTime;
	glm::vec3 worldUp = glm::vec3(0, 1, 0);
	glm::vec3 worldRight = glm::vec3(1, 0, 0);

	float objectRotateY = 0;
	float objectRotateX = 0;

	void ObjectRotateLeft();
	void ObjectRotateRight();
	void ObjectRotateUp();
	void ObjectRotateDown();
};