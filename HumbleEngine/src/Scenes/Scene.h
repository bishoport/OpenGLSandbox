#pragma once
#include "../ActorSystem/Actor.h"
#include <tools/Camera.h>
class Scene {
	void SetUp(libCore::Camera* camera, Ref<Actor> actor);

	void QuickActor(Ref<Actor> actor, Ref<Mesh> model, Ref<Texture> texture, libCore::Shader shader, libCore::Camera* camera,
        glm::vec3 position = glm::vec3(0, 0, 0),
        glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 color = glm::vec3(1, 1, 1),
        bool isColliderAdded = false);

    std::vector<Ref<Actor>> scenesActors;

};
