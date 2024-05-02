#include "Scene.h"
#include "../ActorSystem/Components/RendererComponent.h"

void Scene::SetUp(Ref<libCore::Camera> newCamera)
{
	camera = newCamera;
	gameMode = std::make_shared<GameMode>();

	testMesh1 = libCore::PrimitivesHelper::CreateCube();
	testMesh2 = libCore::PrimitivesHelper::CreateSphere(1.0, 20, 20);

	//-- PREPARE TEXTURES
	std::string texturesDirectory = "C:/Users/migue/Desktop/Projects/OpenGLSandbox/HumbleEngine/assets/textures/";
	diffuse1 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_1.jpg").c_str(), "diffuse", 0);
	diffuse2 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_2.jpg").c_str(), "diffuse", 0);


	std::string shadersDirectory = "C:/Users/migue/Desktop/Projects/OpenGLSandbox/HumbleEngine/assets/shaders/";
	shaderManager.setShaderDataLoad("basic", shadersDirectory + "basic.vert", shadersDirectory + "basic.frag");
	shaderManager.setShaderDataLoad("text", shadersDirectory + "text.vert", shadersDirectory + "text.frag");
	shaderManager.LoadAllShaders();

	Ref<Actor> cubeActor = std::make_shared<Actor>("Cube");
	Ref<libCore::Shader> basicShader = Ref<libCore::Shader>(shaderManager.Get("basic"));
	QuickActor(cubeActor, testMesh1, diffuse1, basicShader);

	//-----------------------------------------------------------------
}

void Scene::QuickActor(Ref<Actor> actor, Ref<Mesh> model, Ref<Texture> texture, Ref<libCore::Shader> shader, glm::vec3 position, glm::vec3 scale, glm::vec3 color, bool isColliderAdded)
{
	Ref<RendererComponent> newRenderer = std::make_shared<RendererComponent>();
	actor->AddComponent(newRenderer);
	Actor* newActor = dynamic_cast<Actor*>(Ref<Actor>(actor).get());
	Ref<Actor> smartPointer = std::make_shared<Actor>(*newActor);

	if (smartPointer != nullptr)
		std::cout << "New Actor available " << smartPointer->ActorName << std::endl;
	actor->ListComponents();
	actor->GetComponent<RendererComponent>()->SetModelInfo(model, shader, camera, texture);
	actor->GetComponent<TransformComponent>()->Ping("Funciona Transform");
	
	//actor->GetComponent<RendererComponent>().SetModelInfo(model, shader, nullptr);
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


