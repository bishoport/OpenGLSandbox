#include "../Components/RendererComponent.hpp"
#include "RenderSystem.h"

libCore::ImportModelData RenderSystem::PrepareGeometry(entt::registry& registry, entt::entity& entity)
{
    auto view = registry.view<Renderer>();
    auto& renderer = view.get<Renderer>(entity);
    renderer.importModelData.filePath = renderer.pathModel;
    renderer.importModelData.fileName = renderer.NameModel;
    renderer.importModelData.invertUV = false;
    renderer.importModelData.rotate90 = false;
    renderer.importModelData.useCustomTransform = true;
    renderer.importModelData.modelID = 1;
    renderer.importModelData.globalScaleFactor = 1.0f;
    return  renderer.importModelData;
    
}
