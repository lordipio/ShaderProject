#pragma once
#include <iostream>
#include <glad/glad.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/scalar_constants.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

class Shader
{

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	GLuint program;
	void Use();
	void SetFloat(const std::string& varName, const float& var);
	void SetInt(const std::string& varName, const int& var);
	void SetBool(const std::string& varName, const bool& var);
	void Set2FloatVector(const std::string& varName, const glm::vec2& var);
	void Set3FloatVector(const std::string& varName, const glm::vec3& var);
	void Set3x3FloatMatrix(const std::string& varName, const glm::mat3x3& var);
	void Set4x4FloatMatrix(const std::string& varName, const glm::mat4x4& var);
	void SetTexture(const std::string& varName, int textureUnit, const GLuint& textureID);
	static void LoadTexture(const char* path, GLuint& textureID);

	~Shader();
private:
	std::string LoadShaderFile(std::string shaderPath);
	GLuint CreateShader(std::string shaderPath, GLuint shaderType);
	void CreateProgram(GLuint vertexShader, GLuint fragShader);
	static void ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height);

};

