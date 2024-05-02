#include "RendererComponent.h"

RendererComponent::RendererComponent() : ActorComponent()
{
//	CurrentCamera = std::make_shared<libCore::Camera>();

}

glm::mat4 RendererComponent::projection()
{
	return glm::perspective(glm::radians(45.f), (float)1024 / (float)720, 0.1f, 100.0f);
}

glm::mat4 RendererComponent::view()
{
	return CurrentCamera->cameraMatrix;
}

glm::mat4 RendererComponent::model()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Transform->position);
	model = glm::rotate(model, Transform->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, Transform->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, Transform->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, Transform->scale);
	return model;
}

void RendererComponent::Begin()
{
}

void RendererComponent::Tick(float deltaTime)
{
	RendererProcess(false);
}

void RendererComponent::RendererProcess(bool isLightingOnlyBool)
{
	glm::mat4 identityMatrix = glm::mat4(1.0f);// La matriz de identidad
   glm::vec3 position(2.0f, 0.0f, -10.0f);// La posición a la que quieres trasladar la matriz 
   glm::mat4 translatedMatrix = glm::translate(identityMatrix, position);// Crear una nueva matriz con la traslación aplicada
	libCore::ShaderManager::Get("basic")->use();
	libCore::ShaderManager::Get("basic")->setMat4("camMatrix", view());
	libCore::ShaderManager::Get("basic")->setMat4("model", glm::mat4(1.0f));
	glActiveTexture(GL_TEXTURE0 + mainTexture->unit);
	glBindTexture(GL_TEXTURE_2D, mainTexture->ID);
	libCore::ShaderManager::Get("basic")->setInt("diffuseTexture", mainTexture->unit);
	CurrentMesh->Draw();
}

void RendererComponent::SetModelInfo(Ref<Mesh> model, Ref<libCore::Shader> shader, Ref<libCore::Camera> camera, Ref<Texture> texture)
{
	CurrentMesh = model;
	CurrentShader = shader;
	CurrentCamera = camera;
	mainTexture = texture;
}
