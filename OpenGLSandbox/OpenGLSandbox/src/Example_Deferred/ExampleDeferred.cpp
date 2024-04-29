#include "ExampleDeferred.h"
#include <input/InputManager.h>
#include <tools/TextureManager.h>
#include <tools/ModelLoader.h>


void ExampleDeferred::Init()
{
    //--SETUP OPENGL & CALLBACKS
    bool ok = libCore::InitializeEngine("Deferred Example", screenWidth, screenHeight,
        std::bind(&ExampleDeferred::LoopOpenGL, this, std::placeholders::_1),
        std::bind(&ExampleDeferred::OnCloseOpenGL, this),
        std::bind(&ExampleDeferred::LoopImGUI, this));
    if (!ok) return;
    //-----------------------------------------------------------------

    //--OPENGL FLAGS
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // -----------------------------

    //--SHADERS
    std::string shadersDirectory = "assets/shaders/";
    shaderManager.setShaderDataLoad("basic",        shadersDirectory + "basic.vert",    shadersDirectory + "basic.frag");
    shaderManager.setShaderDataLoad("colorQuadFBO", shadersDirectory + "quad_fbo.vert", shadersDirectory + "color_quad_fbo.frag");
    shaderManager.setShaderDataLoad("depthQuadFBO", shadersDirectory + "quad_fbo.vert", shadersDirectory + "depth_quad_fbo.frag");
    shaderManager.LoadAllShaders();
    //-----------------------------------------------------------------

    //-- PREPARE GEOMETRY
    libCore::ImportModelData importModelData;
    importModelData.filePath = "assets/models/Robot/";
    importModelData.fileName = "Robot.fbx";
    importModelData.invertUV = false;
    importModelData.rotate90 = false;
    importModelData.modelID = 0;
    m_modelRobot = libCore::ModelLoader::LoadModel(importModelData);
    //-----------------------------------------------------------------

    //-- PREPARE GEOMETRY
    testMesh1 = libCore::PrimitivesHelper::CreateCube();
    testMesh2 = libCore::PrimitivesHelper::CreateSphere(1.0, 20, 20);
    //-----------------------------------------------------------------

    //-- PREPARE TEXTURES
    std::string texturesDirectory = "assets/textures/";
    diffuse2 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_1.jpg").c_str(), "diffuse", 0);
    diffuse3 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_2.jpg").c_str(), "diffuse", 0);
    //-----------------------------------------------------------------

    //-- CAMERA
    m_camera = new libCore::Camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.0f, 5.0f));
    //-----------------------------------------------------------------

    // -- FRAME BUFFER
    fboManager = CreateScope<libCore::FBOManager>();
    fboManager->init(screenWidth, screenHeight);
    //------------------------------------------------------------------

    // -- GBuffer
    //gBufferManager = CreateScope<libCore::GBufferManager>();
    //gBufferManager->init(screenWidth, screenHeight);
    //------------------------------------------------------------------
    

    //-- START LOOP OpenGL
    libCore::InitializeMainLoop();
    //------------------------------------------------------------------
}

void ExampleDeferred::LoopOpenGL(libCore::Timestep deltaTime)
{
    // Update CAMERA
    m_camera->Inputs(deltaTime);
    m_camera->updateMatrix(45.0f, 0.1f, 1000.0f);
    
    fboManager->bindFBO();

    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // Clear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //-- Normal Draw
    libCore::ShaderManager::Get("basic")->use();
    libCore::ShaderManager::Get("basic")->setMat4("camMatrix", m_camera->cameraMatrix);
    m_modelRobot->Draw("basic");
    //---------------------------

    fboManager->unbindFBO();   
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


   // Dibujar color attachment
   libCore::ShaderManager::Get("colorQuadFBO")->use();
   libCore::ShaderManager::Get("colorQuadFBO")->setInt("screenTexture", 0);
   libCore::ShaderManager::Get("colorQuadFBO")->setInt("depthTexture" , 1);
   libCore::ShaderManager::Get("colorQuadFBO")->setFloat("focusDepth" , focusDepth);
   libCore::ShaderManager::Get("colorQuadFBO")->setFloat("blurRadius" , blurRadius);
   fboManager->bindTexture("color",0);
   fboManager->bindTexture("depth",1);
   fboManager->drawFBO("color");


    // Si necesitas visualizar el depth attachment:
    //libCore::ShaderManager::Get("depthQuadFBO")->use();
    //libCore::ShaderManager::Get("depthQuadFBO")->setFloat("near", 0.1f);
    //libCore::ShaderManager::Get("depthQuadFBO")->setFloat("far", 5.0f);
    //libCore::ShaderManager::Get("depthQuadFBO")->setInt("depthTexture", 0);
    //fboManager->drawFBO("depth");
}

void ExampleDeferred::LoopImGUI()
{
    // Crear una ventana ImGui y un panel
    ImGui::Begin("Float Values Control Panel"); // Comienza una ventana nueva

    // Slider para el primer valor float
    ImGui::SliderFloat("focusDepth", &focusDepth, 0.0f, 2.0f, "Value 1: %.3f");

    // Slider para el segundo valor float
    ImGui::SliderFloat("blurRadius", &blurRadius, 0.0f, 1.0f, "Value 2: %.3f");

    ImGui::End(); // Finaliza la ventana
}

void ExampleDeferred::OnCloseOpenGL()
{
}

