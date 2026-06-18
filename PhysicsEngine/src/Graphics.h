#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
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
    void InitWindow();

    GLuint CreateShader(std::string path, GLenum type);
    void CreateProgram(GLuint vs, GLuint fs);
    std::string LoadShaderFile(std::string path);

    void CreateBuffers();
    void UploadMesh(const Mesh& mesh);

    void LoadMesh(const std::string& path);

    void SetMVP();
    void PreDraw();
    void Draw();

private:
    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;

    GLuint program = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint IBO = 0;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;

    Mesh mesh;
};