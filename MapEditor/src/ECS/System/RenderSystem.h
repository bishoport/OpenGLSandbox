#pragma once
#include <entt.hpp>
#include <tools/ModelLoader.h>

class RenderSystem {

public:
	libCore::ImportModelData PrepareGeometry(entt::registry& registry, entt::entity& entity);
};