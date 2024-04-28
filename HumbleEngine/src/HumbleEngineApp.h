#pragma once

#include <LibCore.h>
#include <tools/FreeTypeManager.h>
#include <tools/Camera.h>
#include "ActorSystem/Actor.h"
#include "Scenes/Scene.h"

class HumbleEngineApp {

public:
	HumbleEngineApp() = default;

	void Init();

private:
  
    Ref<Scene> currentScene;
    libCore::ShaderManager shaderManager;
    libCore::FreeTypeManager* freeTypeManager = nullptr;
    libCore::Camera* m_camera = nullptr;

    float lastFrameTime = 0.0f;

    void LoopOpenGL(libCore::Timestep deltaTime);
    void OnCloseOpenGL();
    void LoopImGUI();

};