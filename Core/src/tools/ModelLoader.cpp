#include "ModelLoader.h"
#include "TextureManager.h"

namespace libCore
{
    Ref<ModelContainer> ModelLoader::LoadModel(ImportModelData importOptions)
    {
        auto modelContainer = CreateRef<ModelContainer>();

        Assimp::Importer importer;

        std::string completePath = importOptions.filePath + importOptions.fileName;
        std::cout << "Loading Model ->" << completePath << std::endl;

        unsigned int flags = aiProcess_Triangulate;

        flags |= aiProcess_CalcTangentSpace;
        flags |= aiProcess_GenSmoothNormals;
        flags |= aiProcess_ValidateDataStructure;
        flags |= aiProcess_GenBoundingBoxes;

        if (importOptions.invertUV == true)
        {
            flags |= aiProcess_FlipUVs;
        }

        const aiScene* scene = importer.ReadFile(completePath, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            throw std::runtime_error("Failed to load model");
        }

        aiMatrix4x4 nodeTransform = scene->mRootNode->mTransformation;

        modelContainer->name = importOptions.fileName;

        ModelLoader::processNode(scene->mRootNode, scene, modelContainer, nodeTransform, importOptions);

        return modelContainer;
    }

    void ModelLoader::processNode(aiNode* node, const aiScene* scene, Ref<ModelContainer> modelContainer, aiMatrix4x4 _nodeTransform, ImportModelData importOptions)
    {
        // Convertir a glm
        glm::mat4 glmNodeTransform = aiMatrix4x4ToGlm(_nodeTransform);
        glm::mat4 glmNodeTransformation = aiMatrix4x4ToGlm(node->mTransformation);

        //--Inversion del eje Y<->Z
        float globalRotationDeg_X = 0.0f;
        if (importOptions.rotate90)
        {
            globalRotationDeg_X = -90.0f;
        }
        // Crear una matriz de rotación para la rotación de -90 grados alrededor del eje X
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(globalRotationDeg_X), glm::vec3(1.0f, 0.0f, 0.0f));

        // Aplicar la rotación antes de la transformación del nodo
        glm::mat4 glmFinalTransform = rotationX * glmNodeTransform * glmNodeTransformation;

        // Convertir de nuevo a Assimp
        aiMatrix4x4 finalTransform = glmToAiMatrix4x4(glmFinalTransform);


        //// Procesa todas las mallas (si las hay) en este nodo
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            auto modelBuild = CreateRef<Model>();

            modelBuild->meshes.push_back(ModelLoader::processMesh(mesh, scene, finalTransform, importOptions));
            modelBuild->materials.push_back(processMaterials(mesh, scene, importOptions));

            modelContainer->models.push_back(modelBuild);
        }

        // Luego haz lo mismo para cada uno de sus hijos
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ModelLoader::processNode(node->mChildren[i], scene, modelContainer, _nodeTransform, importOptions);
        }
    }

    Ref<Mesh> ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 finalTransform, ImportModelData importOptions)
    {
        auto meshBuild = CreateRef<Mesh>();

        meshBuild->meshLocalPosition = glm::vec3(finalTransform.a4, finalTransform.b4, finalTransform.c4);


        //Reset de la posicion original para que nos devuelva la matriz en la posicion 0,0,0
        finalTransform.a4 = 0.0;
        finalTransform.b4 = 0.0;
        finalTransform.c4 = 0.0;

        // Inicialización de minBounds y maxBounds con el primer vértice transformado
        /*if (mesh->mNumVertices > 0) {
            glm::vec4 firstVertex = aiMatrix4x4ToGlm(finalTransform) * glm::vec4(
                mesh->mVertices[0].x,
                mesh->mVertices[0].y,
                mesh->mVertices[0].z,
                1);
            meshBuild->minBounds = glm::vec3(firstVertex.x, firstVertex.y, firstVertex.z);
            meshBuild->maxBounds = meshData->minBounds;
        }*/


        // Cargando los datos de los vértices y los índices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            auto vertex = std::make_shared<Vertex>();

            //--Vertex Position
            glm::vec4 posFixed = aiMatrix4x4ToGlm(finalTransform) * glm::vec4(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z,
                1);

            vertex->position = glm::vec3(posFixed.x, posFixed.y, posFixed.z);
            //vertex->Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
            meshBuild->vertexBuffer.push_back(posFixed.x);
            meshBuild->vertexBuffer.push_back(posFixed.y);
            meshBuild->vertexBuffer.push_back(posFixed.z);
            //--------------------------------------------------------------



            //--Texture Coords
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex->texUV = vec;
            }
            else
            {
                vertex->texUV = glm::vec2(0.0f, 0.0f);
            }
                
            meshBuild->vertexBuffer.push_back(vertex->texUV.x);
            meshBuild->vertexBuffer.push_back(vertex->texUV.y);
            //--------------------------------------------------------------


            //--Vertex Normal
            glm::vec4 normFixed = aiMatrix4x4ToGlm(finalTransform) * glm::vec4(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z,
                1);

            vertex->normal = glm::vec3(normFixed.x, normFixed.y, normFixed.z);
            meshBuild->vertexBuffer.push_back(normFixed.x);
            meshBuild->vertexBuffer.push_back(normFixed.y);
            meshBuild->vertexBuffer.push_back(normFixed.z);
            //--------------------------------------------------------------

            //meshBuild->vertices.push_back(vertex);


            // Actualización de minBounds y maxBounds
            /*glm::vec4 posFinal = aiMatrix4x4ToGlm(finalTransform) * glm::vec4(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z,
                1);
            glm::vec3 transformedVertex = glm::vec3(posFinal.x, posFinal.y, posFinal.z);
            meshData->minBounds = glm::min(meshData->minBounds, transformedVertex);
            meshData->maxBounds = glm::max(meshData->maxBounds, transformedVertex);*/
        }


        //-INDICES
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                meshBuild->indices.push_back(face.mIndices[j]);
            }
        }
        //meshBuild->indexCount = meshBuild->indices.size();


        //-MESH ID
        std::string meshNameBase = mesh->mName.C_Str();
        meshNameBase.append(" id:");
        meshBuild->meshName = meshNameBase + std::to_string(importOptions.modelID);

        meshBuild->SetupMesh();

        //meshData->PrepareAABB();
        return meshBuild;
    }

    Ref<Material> ModelLoader::processMaterials(aiMesh* mesh, const aiScene* scene, ImportModelData importOptions)
    {
        auto material = CreateRef<Material>();

        //COLOR DIFUSSE
        aiColor3D color(0.f, 0.f, 0.f);
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);

        material->materialName = mat->GetName().C_Str();

        material->color.r = color.r;
        material->color.g = color.g;
        material->color.b = color.b;

        // Agregamos la carga de la textura ALBEDO aquí
        aiString texturePath;
        
        
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
        {
            std::string completePathTexture = importOptions.filePath + texturePath.C_Str();

            std::cout << "Loading Texture->" << completePathTexture << std::endl;

            Ref<Texture> texture = libCore::TextureManager::LoadTexture(completePathTexture.c_str(), "diffuse", 0);

            if (texture != nullptr) {
                //texture->image->path = completePathTexture;
                material->albedoMap = texture;
            }
            else
            {
                //Ref<Texture> texture = libCore::TextureManager::LoadTexture(completePathTexture.c_str(), "diffuse", 0);
                //std::string defaultPathTexture = "assets/default/default_white.jpg";
                /*texture->image = GLCore::Utils::ImageLoader::loadImage(defaultPathTexture);
                texture->image->path = defaultPathTexture;
                texture->hasMap = true;*/
                material->albedoMap = texture;
            }
        }
        else
        {
            //std::cout << "NO HAY DIFUSO" << std::endl;
            //std::string defaultPathTexture = "assets/default/default_white.jpg";
            //texture->image = GLCore::Utils::ImageLoader::loadImage(defaultPathTexture);
            //texture->image->path = defaultPathTexture;
            //texture->hasMap = true;
            //materialData->albedoMap = texture;
        }

        return material;
    }


    //TOOLS
    glm::mat4 ModelLoader::aiMatrix4x4ToGlm(const aiMatrix4x4& from)
    {
        glm::mat4 to;

        // Transponer y convertir a glm
        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

        return to;
    }
    aiMatrix4x4 ModelLoader::glmToAiMatrix4x4(const glm::mat4& from)
    {
        aiMatrix4x4 to;

        // Transponer y convertir a Assimp
        to.a1 = from[0][0]; to.a2 = from[1][0]; to.a3 = from[2][0]; to.a4 = from[3][0];
        to.b1 = from[0][1]; to.b2 = from[1][1]; to.b3 = from[2][1]; to.b4 = from[3][1];
        to.c1 = from[0][2]; to.c2 = from[1][2]; to.c3 = from[2][2]; to.c4 = from[3][2];
        to.d1 = from[0][3]; to.d2 = from[1][3]; to.d3 = from[2][3]; to.d4 = from[3][3];

        return to;
    }
}
