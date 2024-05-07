#include "Model.h"

namespace libCore
{

	void Model::Draw(const std::string& shader)
	{
		for (unsigned int i = 0; i < materials.size(); i++)
		{
			materials[i]->albedoMap->Bind(shader);
		}

		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			libCore::ShaderManager::Get(shader)->setMat4("model", transform.getMatrix());
			meshes[i]->Draw();
		}
	}


	//void Model::SetupUnifyMeshes() {
	//	unifiedMesh = CreateRef<Mesh>();

	//	for (const auto& mesh : meshes) {
	//		unifiedMesh->vertices.insert(unifiedMesh->vertices.end(), mesh->vertices.begin(), mesh->vertices.end());
	//		unifiedMesh->indices.insert(unifiedMesh->indices.end(), mesh->indices.begin(), mesh->indices.end());
	//	}

	//	unifiedMesh->SetupMesh();  // Sube el `mesh` unificado a la GPU.
	//}
}
