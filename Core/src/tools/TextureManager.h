#pragma once
#include "../LibCoreHeaders.h"
#include "../Core/Texture.h"
#include "../tools/AssetsManager.h"  // Aseg�rate de incluir el encabezado

namespace libCore
{
	class TextureManager
	{
	public:
		static Ref<Texture> LoadTexture(const char* imagePath, const char* texType, GLuint slot);
	};
}