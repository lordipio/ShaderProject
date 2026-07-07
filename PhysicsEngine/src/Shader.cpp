#include "Shader.h"
#include "stb_image.h"
#include <sstream>
#include <fstream>

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    GLuint vertexShader = CreateShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = CreateShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    CreateProgram(vertexShader, fragmentShader);
}

void Shader::CreateProgram(GLuint vertexShader, GLuint fragShader)
{
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
}

void Shader::Use()
{
    glUseProgram(program);
}

GLuint Shader::CreateShader(std::string shaderPath, GLuint shaderType)
{
    std::string shaderSource = LoadShaderFile(shaderPath);
    GLuint shader = glCreateShader(shaderType);
    const char* shaderSourceChar = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceChar, nullptr);
    glCompileShader(shader);
    return shader;
}

std::string Shader::LoadShaderFile(std::string shaderPath)
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


void Shader::SetFloat(const std::string& varName, const float& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniform1f(varLoc, var);
}

void Shader::SetInt(const std::string& varName, const int& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniform1i(varLoc, var);
}

void Shader::SetBool(const std::string& varName, const bool& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniform1f(varLoc, (int)var);
}

void Shader::Set2FloatVector(const std::string& varName, const glm::vec2& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1) 
        glUniform2fv(varLoc, 1, glm::value_ptr(var));
}

void Shader::Set3FloatVector(const std::string& varName, const glm::vec3& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniform3fv(varLoc, 1, glm::value_ptr(var));
}

void Shader::Set3x3FloatMatrix(const std::string& varName, const glm::mat3x3& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniformMatrix3fv(varLoc, 1, GL_FALSE, glm::value_ptr(var));
}

void Shader::Set4x4FloatMatrix(const std::string& varName, const glm::mat4x4& var)
{
    GLuint varLoc = glGetUniformLocation(program, varName.c_str());
    if (varLoc != -1)
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(var));
}

void Shader::SetTexture(const std::string& varName, int textureUnit, const GLuint& textureID)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLuint textureLoc = glGetUniformLocation(program, varName.c_str());

    if (textureLoc != -1)
        glUniform1i(textureLoc, textureUnit);
}


void Shader::LoadTexture(const char* path, GLuint& textureID)
{
    int textureWidth, textureHeight;
    GLuint colorFormat;
    unsigned char* data;
    ReadTextureFile(path, data, colorFormat, textureWidth, textureHeight);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, textureWidth, textureHeight, 0, colorFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}


void Shader::ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height)
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


Shader::~Shader()
{
}

