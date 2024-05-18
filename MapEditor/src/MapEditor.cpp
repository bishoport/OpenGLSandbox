#include "MapEditor.h"
#include <Core/EngineOpenGL.h>


void MapEditor::Init()
{
    bool ok = libCore::EngineOpenGL::GetInstance().InitializeEngine("MAP EDITOR", screenWidth, screenHeight,
        std::bind(&MapEditor::LoopOpenGL, this, std::placeholders::_1),
        std::bind(&MapEditor::OnCloseOpenGL, this),
        std::bind(&MapEditor::LoopImGUI, this));
    if (!ok) return;
    //-----------------------------------------------------------------

    // -- OPENGL FLAGS
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //-----------------------------

    // -- SHADERS
    std::string shadersDirectory = "assets/shaders/";
    shaderManager.setShaderDataLoad("basic", shadersDirectory + "basic.vert", shadersDirectory + "basic.frag");
    shaderManager.setShaderDataLoad("depthQuadFBO", shadersDirectory + "quad_fbo.vert", shadersDirectory + "depth_quad_fbo.frag");
    shaderManager.setShaderDataLoad("colorQuadFBO", shadersDirectory + "quad_fbo.vert", shadersDirectory + "color_quad_fbo.frag");
    shaderManager.LoadAllShaders();
    //-----------------------------------------------------------------


    // -- PREPARE GEOMETRY
    libCore::ImportModelData importModelData;
    importModelData.filePath = "assets/models/Robot/";
    importModelData.fileName = "Robot.fbx";
    importModelData.invertUV = false;
    importModelData.rotate90 = false;
    importModelData.useCustomTransform = true;
    importModelData.modelID = 1;
    importModelData.globalScaleFactor = 1.0f;
    LoadModelInScene(importModelData);
    //-----------------------------------------------------------------


    // -- VIEWPORTS
    libCore::EngineOpenGL::GetInstance().CreateViewport("VIEWPORT_1", glm::vec3(0.0f, 0.0f, 1.0f));
    libCore::EngineOpenGL::GetInstance().CreateViewport("VIEWPORT_2", glm::vec3(0.0f, 0.0f, 5.0f));
    //------------------------------------------------------------------


    // -- START LOOP OpenGL
    libCore::EngineOpenGL::GetInstance().InitializeMainLoop();
    //------------------------------------------------------------------
}



void MapEditor::LoopOpenGL(libCore::Timestep deltaTime)
{
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    
    //--------------------------------------------------------------------------------------------------------
    libCore::EngineOpenGL::GetInstance().RenderViewports(modelsInScene);
    //--------------------------------------------------------------------------------------------------------

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);   
}

void MapEditor::LoopImGUI()
{
    libCore::EngineOpenGL::GetInstance().DrawViewports();
    libCore::EngineOpenGL::GetInstance().DrawHierarchyPanel(modelsInScene);
}

void MapEditor::OnCloseOpenGL()
{
}

void MapEditor::LoadModelInScene(libCore::ImportModelData importModelData)
{
    modelsInScene.push_back(libCore::ModelLoader::LoadModel(importModelData));
}