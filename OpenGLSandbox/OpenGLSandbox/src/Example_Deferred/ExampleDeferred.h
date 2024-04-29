#pragma once

#include <LibCore.h>
#include <tools/ShaderManager.h>
#include <tools/Camera.h>
#include <tools/PrimitivesHelper.h>
#include <tools/ModelLoader.h>
#include <tools/FBOManager.h>
#include <tools/GBufferManager.h>

class ExampleDeferred
{
public:
    ExampleDeferred() = default;
    void Init();

private:

    int screenWidth  = 800;
    int screenHeight = 600;

    Scope<libCore::FBOManager> fboManager;

    libCore::ShaderManager shaderManager;

    float lastFrameTime = 0.0f;

    libCore::Camera* m_camera = nullptr;

    void LoopOpenGL(libCore::Timestep deltaTime);
    void LoopImGUI();
    void OnCloseOpenGL();


    Ref<Texture> diffuse1;
    Ref <libCore::ModelContainer> m_modelRobot;

    Ref<Texture> diffuse2;
    Ref<Texture> diffuse3;

    Ref<Mesh> testMesh1;
    Ref<Mesh> testMesh2;

    //G-Buffer
    unsigned int gBuffer;
    unsigned int gPosition, gNormal, gAlbedoSpec;
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    unsigned int rboDepth;

    // lighting info
    // -------------
    const unsigned int NR_LIGHTS = 32;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    float focusDepth = 0.0f;
    float blurRadius = 0.0f;
};
