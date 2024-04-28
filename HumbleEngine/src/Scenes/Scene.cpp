#include "Scene.h"

void Scene::SetUp()
{
	gameMode = std::make_shared<GameMode>();

	testMesh1 = libCore::PrimitivesHelper::CreateCube();
	testMesh2 = libCore::PrimitivesHelper::CreateSphere(1.0, 20, 20);

	//-- PREPARE TEXTURES
	std::string texturesDirectory = "C:/Users/migue/Desktop/Projects/OpenGLSandbox/HumbleEngine/assets/textures/";
	diffuse1 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_1.jpg").c_str(), "diffuse", 0);
	diffuse2 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_2.jpg").c_str(), "diffuse", 0);
	//-----------------------------------------------------------------
}

void Scene::QuickActor(Ref<Actor> actor, Ref<Mesh> model, Ref<Texture> texture, libCore::Shader shader, libCore::Camera* camera, glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool isColliderAdded)
{
	gameMode->scenesActors.emplace_back(actor);
	
}

void Scene::Init()
{
	gameMode->BeginPlay();
}

void Scene::Tick(float deltaTime)
{
	gameMode->PreRenderer();
	gameMode->PostRenderer();

	gameMode->Tick(deltaTime);
}


