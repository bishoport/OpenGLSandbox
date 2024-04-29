#pragma once
#include "../Core/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Core/Material.h"



namespace libCore
{
	//GENERIC PURPOSE STRUCTS
	// Definir un struct para las opciones de importación
	struct ImportModelData {
		std::string filePath;
		std::string fileName;
		int modelID;
		bool invertUV;
		bool rotate90;
	};

	struct ModelContainer
	{
		std::string name;
		std::vector<Ref<Model>> models;

		void Draw(const std::string& shader)
		{
			for (auto& model : models)
			{
				//model->transform.scale = glm::vec3(0.01f, 0.01f, 0.01f);
				model->Draw("basic");
			}
		}
	};





	class ModelLoader 
	{
	public:
		static Ref<ModelContainer> LoadModel(ImportModelData importOptions);

	private:
		static void processNode(aiNode* node, const aiScene* scene, Ref<ModelContainer> modelContainer, aiMatrix4x4 _nodeTransform, ImportModelData importOptions);
		static Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 finalTransform, ImportModelData importOptions);
		static Ref<Material> processMaterials(aiMesh* mesh, const aiScene* scene, ImportModelData importOptions);

		//tools
		static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
		static aiMatrix4x4 glmToAiMatrix4x4(const glm::mat4& from);
	};
}