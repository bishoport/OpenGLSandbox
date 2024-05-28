#include "BaseScene.h"
#include "../ECS/Components/RendererComponent.hpp"


void BaseScene::BeginPlay()
{
	libCore::ImportModelData importModelData;
	entityRobot = registry.create();
	registry.emplace<Renderer>(entityRobot, importModelData, "Robot.fbx", "assets/models/Robot/");
	entityOperator = registry.create();
	registry.emplace<Renderer>(entityOperator, importModelData, "robot_operator.fbx", "assets/models/Robot/");
	entitySecondOperator = registry.create();
	registry.emplace<Renderer>(entitySecondOperator, importModelData, "robot_operator.fbx", "assets/models/Robot/");

	entitiesList.emplace_back(std::make_shared<entt::entity>(entityRobot));
	entitiesList.emplace_back(std::make_shared<entt::entity>(entityOperator));
	entitiesList.emplace_back(std::make_shared<entt::entity>(entitySecondOperator));

//	MapEditor::GetInstance().LoadModelInScene(systemRender.PrepareGeometry(registry));
}

void BaseScene::Tick(float DeltaTime)
{
}
