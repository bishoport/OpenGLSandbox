#pragma once

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Texture.h"

class Mesh
{
public:
	// Initializes the mesh
	Mesh() = default;

	std::vector <GLfloat> vertexBuffer;
	std::vector <GLuint>  indices;
	std::vector <Texture> textures;

	void SetupMesh();
	void Draw ();

private:
	std::vector <Vertex>  vertices;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;
};