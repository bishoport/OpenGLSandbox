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
    Ref<Texture> diffuse1;
    Ref<Texture> diffuse2;

    Ref<Mesh> testMesh1;
    Ref<Mesh> testMesh2;

    libCore::ShaderManager shaderManager;
    libCore::FreeTypeManager* freeTypeManager = nullptr;
    float lastFrameTime = 0.0f;

    libCore::Camera* m_camera = nullptr;
    void LoopOpenGL(libCore::Timestep deltaTime);
    void OnCloseOpenGL();
    void LoopImGUI();

};