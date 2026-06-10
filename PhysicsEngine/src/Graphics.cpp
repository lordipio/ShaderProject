#include "Graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <sstream>
#include <fstream>

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearErrors();\
	x;\
	ASSERT(GLLogError(#x, __FILE__, __LINE__))

static void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}


static bool GLLogError(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

Graphics::Graphics()
{
	Setup();
}

Graphics::~Graphics()
{
}

void Graphics::Render()
{
    PreDraw();
    ClearScreen();
    Draw();
}

void Graphics::Setup()
{
    InitWindow();

    GLuint fragShader = CreateShader("./src/shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);
    GLuint vertexShader = CreateShader("./src/shaders/vertex_shader.glsl", GL_VERTEX_SHADER);
    CreateProgram(vertexShader, fragShader);
    CreateBuffers();
    SetMVP();
    LoadTexture("./assets/pic.jpg", baseTextureID);
    LoadTexture("./assets/overlay.png", overlayTextureID);
}

void Graphics::CreateProgram(GLuint vertexShader, GLuint fragShader)
{
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
}

GLuint Graphics::CreateShader(std::string shaderPath, GLuint shaderType)
{
    std::string shaderSource = LoadShaderFile(shaderPath);
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSourceChar = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceChar, nullptr);
    glCompileShader(shader);
    return shader;
}

std::string Graphics::LoadShaderFile(std::string shaderPath)
{
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        std::cerr << "Could not open shader file: " << shaderPath << std::endl;
        return std::string("");
    }

    std::string shaderSource((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());

    shaderFile.close();

    return shaderSource;
}

void Graphics::SetMVP()
{
    // MODEL
    model = glm::mat4x4(1.f);
    glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
    glm::rotate(model, 0.f, glm::vec3(1));
    glm::scale(model, glm::vec3(1.f));

    // VIEW
    glm::vec3 eye = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 upVector = glm::vec3(0.f, 1.f, 0.f);

    view = glm::lookAt(eye, center, upVector);

    // PROJECTION
    proj = glm::perspective((float)glm::radians(45.f), float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.f);

    // SEND TO SHADER
    // ...
}

void Graphics::InitWindow()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow(
        "OpenGL + SDL + GLAD - Triangle with MVP",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    glContext = SDL_GL_CreateContext(window);

    if (!glContext)
    {
        std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    SDL_GL_SetSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }


    glEnable(GL_DEPTH_TEST);

    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(GLint(0), GLint(0), GLint(WINDOW_WIDTH), GLint(WINDOW_HEIGHT));

    // glClearColor(GLfloat(0.1f), GLfloat(0.1f), GLfloat(0.1f), GLfloat(1.f));
}

void Graphics::CreateBuffers()
{
    CreateVBO();
    CreateVAO();
    CreateIBO();
}

void Graphics::CreateVBO()
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, globalVertices.size() * sizeof(float), globalVertices.data(), GL_STATIC_DRAW);
}

void Graphics::CreateVAO()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Graphics::CreateIBO()
{
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);
}

void Graphics::LoadMesh(std::string path)
{
    std::ifstream file(path);

    if (!file)
        return;
 
    std::string line;
    float vx, vy, vz;
    float u, v;
    float vnx, vny, vnz;
    char slash;
    int v1, t1, vn1;
    int v2, t2, vn2;
    int v3, t3, vn3;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<FaceIndices> facesIndices;
    std::vector<Vertex> vertices;
    Vertex vertex;


    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;

        ss >> type;

        if (type == "v")
        {
            ss >> vx >> vy >> vz;
            positions.push_back(glm::vec3(vx, vy, vz));
            vertex.pos = glm::vec3(vx, vy, vz);
            vertices.push_back(vertex);
        }

        if (type == "vt")
        {
            ss >> u >> v;
            uvs.push_back(glm::vec2(u, v));
        }

        if (type == "vn")
        {
            ss >> vnx >> vny >> vnz;
            normals.push_back(glm::vec3(vnx, vny, vnz));
        }


        if (type == "f")
        {
            ss >> v1 >> slash >> t1 >> slash >> vn1;
            ss >> v2 >> slash >> t2 >> slash >> vn2;
            ss >> v3 >> slash >> t3 >> slash >> vn3;

            FaceIndices faceIndices;
            faceIndices.faceIndices[0].vertexIndex.r = v1;
            faceIndices.faceIndices[0].vertexIndex.g = t1;
            faceIndices.faceIndices[0].vertexIndex.b = vn1;

            faceIndices.faceIndices[1].vertexIndex.r = v2;
            faceIndices.faceIndices[1].vertexIndex.g = t2;
            faceIndices.faceIndices[1].vertexIndex.b = vn2;

            faceIndices.faceIndices[2].vertexIndex.r = v3;
            faceIndices.faceIndices[2].vertexIndex.g = t3;
            faceIndices.faceIndices[2].vertexIndex.b = vn3;

            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);

            facesIndices.push_back(faceIndices);
        }
    }


    for (int i = 0; i < facesIndices.size(); i++)
    {
        int pos1Index = facesIndices[i].faceIndices[0].vertexIndex.r;
        int uv1Index = facesIndices[i].faceIndices[0].vertexIndex.g;
        int normal1Index = facesIndices[i].faceIndices[0].vertexIndex.b;

        int pos2Index = facesIndices[i].faceIndices[0].vertexIndex.r;
        int uv2Index = facesIndices[i].faceIndices[1].vertexIndex.g;
        int normal2Index = facesIndices[i].faceIndices[1].vertexIndex.b;

        int pos3Index = facesIndices[i].faceIndices[0].vertexIndex.r;
        int uv3Index = facesIndices[i].faceIndices[2].vertexIndex.g;
        int normal3Index = facesIndices[i].faceIndices[2].vertexIndex.b;

        vertices[pos1Index].uvs = uvs[uv1Index];
        vertices[pos1Index].normal = normals[normal1Index];

        vertices[pos2Index].uvs = uvs[uv2Index];
        vertices[pos2Index].normal = normals[normal2Index];

        vertices[pos2Index].uvs = uvs[uv3Index];
        vertices[pos2Index].normal = normals[normal3Index];
    }

    // globalVertexBuffer.insert(globalVertexBuffer.end(), suzanne.vertexBuffer.begin(), suzanne.vertexBuffer.end());
    
}

void Graphics::PreDraw()
{

    // SET TEXTURES
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTextureID);
    GLuint diffuseTextureLoc = glGetUniformLocation(program, "diffuse");

    if (diffuseTextureLoc != -1)
        glUniform1i(diffuseTextureLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlayTextureID);
    GLuint overlayTextureLoc = glGetUniformLocation(program, "overlay");
    
    if (overlayTextureLoc != -1)
        glUniform1i(overlayTextureLoc, 1);

    // SET MODEL
    GLuint modelLoc = glGetUniformLocation(program, "model");
    if (modelLoc != -1)
    {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    }

    // SET VIEW
    GLuint viewLoc = glGetUniformLocation(program, "view");
    if (viewLoc != -1)
    {
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    }

    // SET PROJECTION
    GLuint projLoc = glGetUniformLocation(program, "projection");
    if (projLoc != -1)
    {
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    }

    // SET TINT
    GLuint tintLoc = glGetUniformLocation(program, "tint");
    if (tintLoc != -1)
        glUniform3fv(tintLoc, 1, glm::value_ptr(glm::vec3(0.7, 1, 0.2f)));

    GLuint resolutionLoc = glGetUniformLocation(program, "resolution");
    if (resolutionLoc != -1)
        glUniform2f(resolutionLoc, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLuint timeLoc = glGetUniformLocation(program, "currentTime");
    if (timeLoc != -1)
        glUniform1f(timeLoc, currentTime);
}

void Graphics::ClearScreen()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Graphics::Draw()
{
    glUseProgram(program);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, baseTextureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, overlayTextureID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    SDL_GL_SwapWindow(window);
}

void Graphics::ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height)
{
    int channels;
    data = stbi_load(path, &width, &height, &channels, 0);

    if (!data)
    {
        std::cout << "Texture Data was not found!";
        return;
    }
    colorFormat = channels == 4 ? GL_RGBA : GL_RGB;
}

void Graphics::LoadTexture(const char* path, GLuint& textureID)
{
    int textureWidth, textureHeight;
    GLuint colorFormat;
    unsigned char* data;
    ReadTextureFile(path, data, colorFormat, textureWidth, textureHeight);


    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    
    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Graphics::Update(float dt)
{
    currentTime += dt;
}
