#include "Model.h"

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
