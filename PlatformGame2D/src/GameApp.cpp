#include "GameApp.h"

void GameApp::Init()
{
    //--SETUP OPENGL & CALLBACKS
    bool ok = libCore::InitializeEngine("2D Platform Game", 1080, 720,
        std::bind(&GameApp::LoopOpenGL, this, std::placeholders::_1),
        std::bind(&GameApp::OnCloseOpenGL, this),
        std::bind(&GameApp::LoopImGUI, this));
    if (!ok) return;
    //-----------------------------------------------------------------

    //--OpenGL FLAGS
        //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //---------------------------------------------------------------------------

    //-- OTHERS
    shaderManager.LoadAllShaders();

    ////-- PHYSICS
    //b2Vec2 gravity(0.0f, -10.0f); // Gravedad hacia abajo
    //world = new b2World(gravity);


    ////-- PLAYER
    //CharacterData data{ "Nombre del Personaje" };
    //player = new Character(data, world);




    ////GenerateMap();
    //GenerateLevel(Game::ImageLoader::LoadTexture("Assets/maps/TestMap2/static_layer.png"), false);
    ////GenerateLevel(Game::ImageLoader::LoadTexture("Assets/maps/TestMap2/dinamic_layer.png"), true);

    //--START LOOP OpenGL
    libCore::InitializeMainLoop();
    //----------------------------------------------------------------------------
}

void GameApp::LoopOpenGL(libCore::Timestep deltaTime)
{
}

void GameApp::OnCloseOpenGL()
{
}

void GameApp::LoopImGUI()
{
}
