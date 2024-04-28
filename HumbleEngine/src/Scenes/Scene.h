#pragma once
#include "../ActorSystem/Actor.h"
#include <tools/Camera.h>
#include <tools/PrimitivesHelper.h>
#include <tools/TextureManager.h>
#include "../GameMode.h"
class Scene {
public:

    Ref<Texture> diffuse1;
    Ref<Texture> diffuse2;

    Ref<Mesh> testMesh1;
    Ref<Mesh> testMesh2;


	void SetUp();

	void QuickActor(Ref<Actor> actor, Ref<Mesh> model, Ref<Texture> texture, libCore::Shader shader, libCore::Camera* camera,
        glm::vec3 position = glm::vec3(0, 0, 0),
        glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 color = glm::vec3(1, 1, 1),
        bool isColliderAdded = false);

    Ref<GameMode> gameMode;

    void Init();

    void Tick(float deltaTime);
};
