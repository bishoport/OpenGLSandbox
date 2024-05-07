#include "AssetsManager.h"
#include "TextureManager.h"

namespace libCore
{
	AssetsManager& assetsManager = AssetsManager::GetInstance();

	void AssetsManager::LoadDefaultAssets()
	{
		std::string defaultAssetsPathTexture = "C:/Users/pdortegon/Documents/PROPIOS/OpenGL_DEV/OpenGLSandbox/Core/assets/textures/";

		Ref<Texture> white_texture = TextureManager::LoadTexture((defaultAssetsPathTexture + "default_white.jpg").c_str(), "diffuse", 0);
		SetTexture("default_white", white_texture);

		Ref<Texture> black_texture = TextureManager::LoadTexture((defaultAssetsPathTexture + "default_black.jpg").c_str(), "diffuse", 0);
		SetTexture("default_black", black_texture);

		Ref<Texture> normal_texture = TextureManager::LoadTexture((defaultAssetsPathTexture + "default_normal.jpg").c_str(), "diffuse", 0);
		SetTexture("default_normal", white_texture);
	}

	// Método para obtener una textura
	Ref<Texture> AssetsManager::GetTexture(const std::string& name) {
		auto it = loadedTextures.find(name);
		if (it != loadedTextures.end()) {
			return it->second;
		}
		else {
			// Manejar el caso en que la textura no se encuentra
			return nullptr;
		}
	}

	void AssetsManager::SetTexture(const std::string& name, const Ref<Texture>& texture)
	{
		loadedTextures[name] = texture;
	}

	// Método para eliminar una textura
	void AssetsManager::UnloadTexture(const std::string& name) {
		loadedTextures.erase(name);
	}
}