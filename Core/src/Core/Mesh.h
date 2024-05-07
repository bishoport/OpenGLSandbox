#pragma once
#include "../LibCoreHeaders.h"
#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Texture.h"

namespace libCore
{
	class Mesh
	{
	public:
		// Initializes the mesh
		Mesh() = default;

		std::string meshName = "unknow_mesh_name";
		glm::vec3 meshLocalPosition = glm::vec3(0.0f);

		std::vector <GLfloat> vertexBuffer;
		std::vector <Vertex>  vertices;
		std::vector <GLuint>  indices;
		std::vector <Texture> textures;

		void SetupMesh();

		//void SetupSkeletalMesh();
		void Draw();

	private:

		// Store VAO in public so it can be used in the Draw function
		VAO VAO;

		//unsigned int VAO,VBO, EBO;
	};
}