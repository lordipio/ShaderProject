#define _CRT_SECURE_NO_WARNINGS
#include "Graphics.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Graphics::Graphics()
{
    Setup();
}

Graphics::~Graphics()
{
}

void Graphics::Setup()
{
    InitWindow();

    GLuint vs = CreateShader("./src/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fs = CreateShader("./src/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    CreateProgram(vs, fs);

    LoadMesh("./assets/suzanne.obj");

    std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "Indices: " << mesh.indices.size() << std::endl;

    CreateBuffers();
    UploadMesh(mesh);

    SetMVP();
}

void Graphics::InitWindow()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow("OBJ Viewer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    context = SDL_GL_CreateContext(window);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

GLuint Graphics::CreateShader(std::string path, GLenum type)
{
    std::string src = LoadShaderFile(path);
    const char* cstr = src.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);

    return shader;
}

void Graphics::CreateProgram(GLuint vs, GLuint fs)
{
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
}

std::string Graphics::LoadShaderFile(std::string path)
{
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Graphics::SetMVP()
{
    model = glm::mat4(1.0f);

    view = glm::lookAt(
        glm::vec3(0, 0, 5),  // move camera back
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );

    proj = glm::perspective(
        glm::radians(45.0f),
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, // FIXED
        0.1f,
        100.0f
    );
}

void Graphics::CreateBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
}

void Graphics::UploadMesh(const Mesh& m)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
        m.vertices.size() * sizeof(Vertex),
        m.vertices.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        m.indices.size() * sizeof(unsigned int),
        m.indices.data(),
        GL_STATIC_DRAW);
}

void Graphics::LoadMesh(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cout << "Failed to open OBJ\n";
        return;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (type == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uvs.push_back(uv);
        }
        else if (type == "vn")
        {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (type == "f")
        {
            std::string vertStr;

            for (int i = 0; i < 3; i++)
            {
                ss >> vertStr;

                int v = 0, t = 0, n = 0;

                if (sscanf(vertStr.c_str(), "%d/%d/%d", &v, &t, &n) == 3)
                {
                }
                else if (sscanf(vertStr.c_str(), "%d//%d", &v, &n) == 2)
                {
                    t = 0;
                }
                else if (sscanf(vertStr.c_str(), "%d/%d", &v, &t) == 2)
                {
                    n = 0;
                }
                else
                {
                    sscanf(vertStr.c_str(), "%d", &v);
                }

                Vertex vert{};
                vert.pos = positions[v - 1];

                if (t > 0 && !uvs.empty())
                    vert.uv = uvs[t - 1];
                else
                    vert.uv = glm::vec2(0.0f);

                if (n > 0 && !normals.empty())
                    vert.normal = normals[n - 1];
                else
                    vert.normal = glm::vec3(0, 0, 1);

                mesh.vertices.push_back(vert);
                mesh.indices.push_back(mesh.vertices.size() - 1);
            }
        }
    }
}

void Graphics::PreDraw()
{
    glUseProgram(program); 

    glUniformMatrix4fv(glGetUniformLocation(program, "model"),
        1, GL_FALSE, glm::value_ptr(model));

    glUniformMatrix4fv(glGetUniformLocation(program, "view"),
        1, GL_FALSE, glm::value_ptr(view));

    glUniformMatrix4fv(glGetUniformLocation(program, "projection"),
        1, GL_FALSE, glm::value_ptr(proj));
}

void Graphics::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES,
        mesh.indices.size(),
        GL_UNSIGNED_INT,
        0);

    SDL_GL_SwapWindow(window);
}

void Graphics::Render()
{
    PreDraw();
    Draw();
}

void Graphics::Update(float dt)
{
}