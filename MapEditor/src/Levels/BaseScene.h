#pragma once
#include "../ECS/System/RenderSystem.h"
class BaseScene {
	private:
		//std::vector<>
		RenderSystem systemRender;
		entt::registry registry;
		entt::entity entityRobot;
		entt::entity entityOperator;
		entt::entity entitySecondOperator;

	protected:

		
	public:
		BaseScene() = default;
		void BeginPlay();

		void Tick(float DeltaTime);

		libCore::ImportModelData ModelData(Ref<entt::entity> entity) {
			return systemRender.PrepareGeometry(registry, *entity.get());
		}

		std::vector<Ref<entt::entity>> entitiesList;
};