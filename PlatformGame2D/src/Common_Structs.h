#pragma once

#include <string>
#include <Headers.h>

namespace Game
{
	enum class CharacterState {
		Idle,
		Run,
		Jump,
		Attack,
		Damage,
		Dead
	};

	struct Image {
		unsigned char* pixels;
		int width = 0;
		int height = 0;
		int channels;
		std::string path = "NONE";
	};


	struct Texture {

		GLuint textureID = 0;

		std::string typeString;
		libopengl::Ref<Image> image;

		void Bind() 
		{

			if (image->pixels == nullptr) {
				std::cerr << "Image data is null!" << std::endl;
				return;
			}

			GLenum format{};
			if (image->channels == 1)
				format = GL_RED;
			else if (image->channels == 3)
				format = GL_RGB;
			else if (image->channels == 4)
				format = GL_RGBA;


			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->pixels);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set the texture wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding

			// Set texture filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Use linear filtering for minification
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Use linear filtering for magnification
		}
	};



	struct Animation
	{
		std::string name = "";
		//unsigned int texture = 0;
		libopengl::Ref<Texture> texture;
		int totalFrames = 15; // Cantidad total de frames en tu sprite sheet
		int currentFrame = 0; // El frame actual que quieres mostrar
		float frameTime = 0.1f;
		float frameWidth = 1.0f / totalFrames; // Ancho de cada frame en coordenadas de textura
		bool loop = true; // Por defecto en loop, cambiar según el estado.


		
	};



	struct CharacterData
	{
		std::string name;
	};

	
	// Definir una estructura para el hash de std::pair<int, int>
	//struct pair_hash {
	//	template <class T1, class T2>
	//	std::size_t operator () (const std::pair<T1, T2>& pair) const {
	//		auto hash1 = std::hash<T1>{}(pair.first);
	//		auto hash2 = std::hash<T2>{}(pair.second);
	//		return hash1 ^ hash2; // Puedes usar otra función de mezcla si lo prefieres
	//	}
	//};

	struct AABB {
		glm::vec3 min; // Esquina inferior del cuadro de colisión
		glm::vec3 max; // Esquina superior del cuadro de colisión
	};

}