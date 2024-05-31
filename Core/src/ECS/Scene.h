#pragma once
#include "EntityManager.h"
#include "Systems/RenderSystem.h"
class Scene {
private:
	RenderSystem systemRender;
public:
	void BeginPlay();

	void Tick(float DeltaTime);

	//libCore::ImportModelData ModelData(Ref<entt::entity> entity) {
	//	return systemRender.PrepareGeometry(EntityManager::GetInstance().m_registry, *entity.get());
	//}

};