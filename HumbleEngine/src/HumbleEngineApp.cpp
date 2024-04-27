#define STB_IMAGE_IMPLEMENTATION
#include "HumbleEngineApp.h"
#include <tools/PrimitivesHelper.h>
#include <tools/TextureManager.h>
#pragma once

void HumbleEngineApp::Init()
{
    //--SETUP OPENGL & CALLBACKS
    bool ok = libCore::InitializeEngine("Humble Studio", 800, 600,
        std::bind(&HumbleEngineApp::LoopOpenGL, this, std::placeholders::_1),
        std::bind(&HumbleEngineApp::OnCloseOpenGL, this),
        std::bind(&HumbleEngineApp::LoopImGUI, this));
    if (!ok) return;
    //---------------------

    std::string shadersDirectory = "C:/Users/migue/Desktop/Projects/OpenGLSandbox/HumbleEngine/assets/shaders/";
    shaderManager.setShaderDataLoad("basic", shadersDirectory + "basic.vert", shadersDirectory + "basic.frag");
    shaderManager.setShaderDataLoad("text", shadersDirectory + "text.vert", shadersDirectory + "text.frag");
    shaderManager.LoadAllShaders();

    freeTypeManager = new libCore::FreeTypeManager();

    //-- PREPARE GEOMETRY
    testMesh1 = libCore::PrimitivesHelper::CreateCube();
    testMesh2 = libCore::PrimitivesHelper::CreateSphere(1.0, 20, 20);
    //-----------------------------------------------------------------

    //-- PREPARE TEXTURES
    std::string texturesDirectory = "C:/Users/migue/Desktop/Projects/OpenGLSandbox/HumbleEngine/assets/textures/";
    diffuse1 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_1.jpg").c_str(), "diffuse", 0);
    diffuse2 = libCore::TextureManager::LoadTexture((texturesDirectory + "floor_2.jpg").c_str(), "diffuse", 0);
    //-----------------------------------------------------------------

    m_camera = new libCore::Camera(800, 600, glm::vec3(0.0f, 0.0f, 5.0f));
    libCore::InitializeMainLoop();


}

void HumbleEngineApp::LoopOpenGL(libCore::Timestep deltaTime)
{
    // Update CAMERA
    m_camera->Inputs(deltaTime);
    m_camera->updateMatrix(45.0f, 0.1f, 100.0f);

    // Clear
    glClearColor(0.3, 0.1, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Stencil 1
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Configura el stencil para pasar siempre
    glStencilMask(0xFF);               // Habilita la escritura al buffer de stencil

    // --Draw first MODEL 
    libCore::ShaderManager::Get("basic")->use();
    libCore::ShaderManager::Get("basic")->setMat4("camMatrix", m_camera->cameraMatrix);
    libCore::ShaderManager::Get("basic")->setMat4("model", glm::mat4(1.0f));
    libCore::ShaderManager::Get("basic")->setInt("diffuseTexture", diffuse1->unit);

    diffuse1->Bind();
    testMesh1->Draw();
    //---------------------------------------------------------------------------------------------------


    // --Draw second MODEL 
    glm::mat4 identityMatrix = glm::mat4(1.0f);// La matriz de identidad
    glm::vec3 position(2.0f, 0.0f, 0.0f);// La posición a la que quieres trasladar la matriz 
    glm::mat4 translatedMatrix = glm::translate(identityMatrix, position);// Crear una nueva matriz con la traslación aplicada

    libCore::ShaderManager::Get("basic")->use();
    libCore::ShaderManager::Get("basic")->setMat4("camMatrix", m_camera->cameraMatrix);
    libCore::ShaderManager::Get("basic")->setMat4("model", translatedMatrix);
    libCore::ShaderManager::Get("basic")->setInt("diffuseTexture", diffuse2->unit);

    diffuse2->Bind();
    testMesh2->Draw();
    //---------------------------------------------------------------------------------------------------
}

void HumbleEngineApp::OnCloseOpenGL()
{
}

void HumbleEngineApp::LoopImGUI()
{
}
