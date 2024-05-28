#pragma once
#include <tools/ModelLoader.h>
#include <Timestep.h>
#include "Singleton.h"
#include "Levels/BaseScene.h"

class MapEditor : public Singleton<MapEditor>
{
public:
    MapEditor() = default;
    void Init();

    

private:

    int screenWidth = 1080;
    int screenHeight = 720;

    libCore::ShaderManager shaderManager;

    float lastFrameTime = 0.0f;

    void LoopOpenGL(libCore::Timestep deltaTime);
    void LoopImGUI();
    void OnCloseOpenGL();





    float focusDepth = 1.0f;
    float blurRadius = 0.019f;
    float blurAmount = 0.302f;
    int   blurSampleCount = 10;

    BaseScene * defaultScene;

public:
    void LoadModelInScene(libCore::ImportModelData importModelData);
     std::vector<Ref<libCore::ModelContainer>> modelsInScene;

};