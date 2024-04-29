#pragma once

#include<stb_image/stb_image.h>
#include<glad/glad.h>
#include"../tools/ShaderManager.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	//// Assigns a texture unit to a texture
	//void texUnit(std::string shaderName, GLuint unit);
	// Binds a texture
	void Bind(const std::string& shader);
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
