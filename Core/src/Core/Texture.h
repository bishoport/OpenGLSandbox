#pragma once

#include<stb_image/stb_image.h>
#include<glad/glad.h>
#include"../tools/ShaderManager.h"

namespace libCore
{
	class Texture
	{
	public:
		GLuint ID;
		const char* type;
		GLuint unit;
		std::string textureName = "";

		Texture(const char* image, const char* texType, GLuint slot);

		// Binds a texture
		void Bind(const std::string& shader);
		// Unbinds a texture
		void Unbind();
		// Deletes a texture
		void Delete();
	};
}
