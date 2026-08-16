#pragma once
#include "Shader.h"

class Material
{
public:

	Material(const std::string& vertexShaderPath = "./src/shaders/vertex_shader.glsl", const std::string& fragmentShaderPath = "./src/shaders/fragment_shader.glsl");
	
	Shader* GetShader() { return shader; }
	void SetShader(Shader* newShader) { if (newShader) shader = newShader; }

	void AddTexture(std::string name, GLuint textureID);
	void AddTexture(std::string name, std::string textureAddress);

	void AddCubemapTexture(std::string name, GLuint textureID);
	void AddCubemapTexture(std::string name, std::string textureAddress[6]);

	void BindTextures();
	static void LoadTexture(const char* path, GLuint& textureID);
	static void LoadCubemapTexture(std::string texturesAddress[6], GLuint& textureID);


	std::unordered_map <std::string, GLuint> GetTextureNameID() { return texturesNameID; }

private:
	Shader* shader;
	std::unordered_map<std::string, GLuint> texturesNameID;

	static void ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height);
};