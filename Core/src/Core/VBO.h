#pragma once

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

// Structure to standardize the vertices used in the meshes
struct Vertex
{
	glm::vec3 position;
	glm::vec2 texUV;
	glm::vec3 normal;
};


//---Vertex Buffer Object(VBO)
//-------------------------------------------------------------------------
//Es un contenedor de memoria en la GPU que almacena datos de vértices.
//Un VBO puede contener información como la posición de los vértices, las coordenadas de textura, las normales, 
//los colores, y otros datos de atributos de vértices que definen la forma y la apariencia de tu objeto 3D.
//----------------------------------------------------------------------------------------------------------------

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(std::vector<Vertex>& vertices);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};