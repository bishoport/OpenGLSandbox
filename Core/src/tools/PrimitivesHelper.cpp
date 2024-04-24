#include "PrimitivesHelper.h"


namespace libCore {

    //#define M_PI 3.14159265358979323846

    Ref<Mesh> PrimitivesHelper::CreateQuad() {

        auto mesh = CreateRef<Mesh>();

        mesh->vertexBuffer = {
            // positions        // texture Coords      // normals
           -0.5f,  0.5f, 0.0f,     0.0f, 0.0f,       0.0f, 0.0f, 1.0f,
           -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,       0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, 0.0f,     1.0f, 1.0f,       0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f,     1.0f, 0.0f,       0.0f, 0.0f, 1.0f,
        };

        mesh->indices = {
            0, 1, 2,
            1, 3, 2
        };

        mesh->SetupMesh();

        return mesh;
    }

    Ref<Mesh> PrimitivesHelper::CreatePlane()
    {
        auto mesh = CreateRef<Mesh>();

        const int divisions = 10;
        const float size = 10.0f; // tamaño del plano completo
        const float step = size / divisions;
        const float halfSize = size * 0.5f;

        for (int i = 0; i <= divisions; i++) {
            for (int j = 0; j <= divisions; j++) {
                // Posiciones
                mesh->vertexBuffer.push_back(j * step - halfSize); // x
                mesh->vertexBuffer.push_back(0.0f);                // y
                mesh->vertexBuffer.push_back(i * step - halfSize); // z

                // Coordenadas de textura
                mesh->vertexBuffer.push_back((float)j / divisions);  // u
                mesh->vertexBuffer.push_back((float)i / divisions);  // v

                // Normales
                mesh->vertexBuffer.push_back(0.0f);
                mesh->vertexBuffer.push_back(1.0f);
                mesh->vertexBuffer.push_back(0.0f);
            }
        }

        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                GLuint topLeft = i * (divisions + 1) + j;
                GLuint topRight = topLeft + 1;
                GLuint bottomLeft = (i + 1) * (divisions + 1) + j;
                GLuint bottomRight = bottomLeft + 1;

                mesh->indices.push_back(topLeft);
                mesh->indices.push_back(bottomLeft);
                mesh->indices.push_back(topRight);

                mesh->indices.push_back(topRight);
                mesh->indices.push_back(bottomLeft);
                mesh->indices.push_back(bottomRight);
            }
        }
        
        mesh->SetupMesh();

        return mesh;
    }

    Ref<Mesh> PrimitivesHelper::CreateCube() {

        auto mesh = CreateRef<Mesh>();

        // Definiendo los 8 vértices del cubo (posición, coordenada de textura y normales)
        mesh->vertexBuffer = {
            // Posiciones      // Coordenadas de Textura        // Normales
             -0.5f,  0.5f, -0.5f,           0, 0,               0.0f,  0.0f, -1.0f,
             -0.5f, -0.5f, -0.5f,           0, 1,               0.0f,  0.0f, -1.0f,
              0.5f, -0.5f, -0.5f,           1, 1,               0.0f,  0.0f, -1.0f,
              0.5f,  0.5f, -0.5f,           1, 0,               0.0f,  0.0f, -1.0f,

             -0.5f,  0.5f,  0.5f,           0, 0,               0.0f,  0.0f,  1.0f,
             -0.5f, -0.5f,  0.5f,           0, 1,               0.0f,  0.0f,  1.0f,
              0.5f, -0.5f,  0.5f,           1, 1,               0.0f,  0.0f,  1.0f,
              0.5f,  0.5f,  0.5f,           1, 0,               0.0f,  0.0f,  1.0f,

              0.5f,  0.5f, -0.5f,           1, 0,               1.0f,  0.0f,  0.0f,
              0.5f, -0.5f, -0.5f,           1, 1,               1.0f,  0.0f,  0.0f,
              0.5f, -0.5f,  0.5f,           0, 1,               1.0f,  0.0f,  0.0f,
              0.5f,  0.5f,  0.5f,           0, 0,               1.0f,  0.0f,  0.0f,

             -0.5f,  0.5f, -0.5f,           1, 0,              -1.0f,  0.0f,  0.0f,
             -0.5f, -0.5f, -0.5f,           1, 1,              -1.0f,  0.0f,  0.0f,
             -0.5f, -0.5f,  0.5f,           0, 1,              -1.0f,  0.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,           0, 0,              -1.0f,  0.0f,  0.0f,

             -0.5f,  0.5f,  0.5f,           0, 0,               0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f, -0.5f,           0, 1,               0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,           1, 1,               0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,           1, 0,               0.0f,  1.0f,  0.0f,

             -0.5f, -0.5f,  0.5f,           0, 0,               0.0f, -1.0f,  0.0f,
             -0.5f, -0.5f, -0.5f,           0, 1,               0.0f, -1.0f,  0.0f,
              0.5f, -0.5f, -0.5f,           1, 1,               0.0f, -1.0f,  0.0f,
              0.5f, -0.5f,  0.5f,           1, 0,               0.0f, -1.0f,  0.0f
        };

        // Definiendo los índices de los triángulos
        mesh->indices = {
             0,  1,  3,   3,  1,  2,
             4,  5,  7,   7,  5,  6,
             8,  9, 11,  11,  9, 10,
            12, 13, 15,  15, 13, 14,
            16, 17, 19,  19, 17, 18,
            20, 21, 23,  23, 21, 22
        };

        mesh->SetupMesh();

        return mesh;
    }

    Ref<Mesh> PrimitivesHelper::CreateSphere(float radius, unsigned int sectorCount, unsigned int stackCount) {

        auto meshData = CreateRef<Mesh>();

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;

        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= sectorCount; ++x)
        {
            for (unsigned int y = 0; y <= stackCount; ++y)
            {
                float xSegment = (float)x / (float)sectorCount;
                float ySegment = (float)y / (float)stackCount;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos) * radius);
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        for (unsigned int y = 0; y < stackCount; ++y)
        {
            for (unsigned int x = 0; x <= sectorCount; ++x)
            {
                meshData->indices.push_back(y * (sectorCount + 1) + x);
                meshData->indices.push_back((y + 1) * (sectorCount + 1) + x);
                meshData->indices.push_back((y + 1) * (sectorCount + 1) + x + 1);
                meshData->indices.push_back(y * (sectorCount + 1) + x);
                meshData->indices.push_back((y + 1) * (sectorCount + 1) + x + 1);
                meshData->indices.push_back(y * (sectorCount + 1) + x + 1);
            }
        }


        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            meshData->vertexBuffer.push_back(positions[i].x);
            meshData->vertexBuffer.push_back(positions[i].y);
            meshData->vertexBuffer.push_back(positions[i].z);

            if (uv.size() > 0)
            {
                meshData->vertexBuffer.push_back(uv[i].x);
                meshData->vertexBuffer.push_back(uv[i].y);
            }

            if (normals.size() > 0)
            {
                meshData->vertexBuffer.push_back(normals[i].x);
                meshData->vertexBuffer.push_back(normals[i].y);
                meshData->vertexBuffer.push_back(normals[i].z);
            }
        }

        meshData->SetupMesh();

        return meshData;
    }
}