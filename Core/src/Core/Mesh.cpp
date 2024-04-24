#include "Mesh.h"


void Mesh::SetupMesh()
{
	vertices.reserve(vertexBuffer.size() / 8); // Dividimos por 8 porque cada vértice tiene 8 floats
	
	for (size_t i = 0; i < vertexBuffer.size(); i += 8) {
		glm::vec3 position(vertexBuffer[i], vertexBuffer[i + 1], vertexBuffer[i + 2]);
		glm::vec2 texUV(vertexBuffer[i + 3], vertexBuffer[i + 4]);
		glm::vec3 normal(vertexBuffer[i + 5], vertexBuffer[i + 6], vertexBuffer[i + 7]);

		vertices.push_back(Vertex{ position, texUV, normal });
	}


	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw()
{
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	//unsigned int numDiffuse = 0;
	//unsigned int numSpecular = 0;

	//for (unsigned int i = 0; i < textures.size(); i++)
	//{
	//	std::string num;
	//	std::string type = textures[i].type;
	//	if (type == "diffuse")
	//	{
	//		num = std::to_string(numDiffuse++);
	//	}
	//	else if (type == "specular")
	//	{
	//		num = std::to_string(numSpecular++);
	//	}
	//	//textures[i].texUnit(shader, (type + num).c_str(), i);
	//	textures[i].Bind();
	//}

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
