#include "Material.h"
#include "stb_image.h"

Material::Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{ 
    shader = new Shader(vertexShaderPath, fragmentShaderPath);
}

void Material::AddTexture(std::string name, GLuint textureID)
{
    texturesNameID.emplace(name, textureID);
}

void Material::AddTexture(std::string name, std::string textureAddress)
{
    GLuint texID;
    LoadTexture(textureAddress.c_str(), texID);
    texturesNameID.emplace(name, texID);
}

void Material::BindTextures()
{
	int texMapIndex = 0;
	int texUnit;
	int texLoc;
    shader->Use();
	for (auto& textureMap : texturesNameID)
	{
		texUnit = GL_TEXTURE0 + texMapIndex;
		glActiveTexture(texUnit);
		glBindTexture(GL_TEXTURE_2D, textureMap.second);
		texLoc = glGetUniformLocation(shader->program, textureMap.first.c_str());
		if (texLoc != -1)
			glUniform1i(texLoc, texUnit);
		texMapIndex++;
	}
}

void Material::LoadTexture(const char* path, GLuint& textureID)
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


void Material::ReadTextureFile(const char* path, unsigned char*& data, GLuint& colorFormat, int& width, int& height)
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
