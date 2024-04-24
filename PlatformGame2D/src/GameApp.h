#pragma once

#include <LibCore.h>
#include <tools/ShaderManager.h>
#include <tools/Camera.h>

class GameApp
{
public:
    GameApp() = default;

    void Init();

private:


    libCore::ShaderManager shaderManager;

    float lastFrameTime = 0.0f;

    libCore::Camera* m_camera = nullptr;
    void LoopOpenGL(libCore::Timestep deltaTime);
    void OnCloseOpenGL();
    void LoopImGUI();

};