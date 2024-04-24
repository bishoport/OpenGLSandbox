#pragma once
#include "../LibCoreHeaders.h"
#include "../Core/Texture.h"

namespace libCore
{
	class TextureManager
	{
	public:
		static Ref<Texture> LoadTexture(const char* image, const char* texType, GLuint slot);


	};

}