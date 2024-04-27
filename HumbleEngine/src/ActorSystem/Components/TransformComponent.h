#pragma once
#include "../ActorComponent.h"
#include <glm/glm.hpp>

class TransformComponent : public ActorComponent {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 pivot;

	TransformComponent();

	glm::vec3 direction();

	void SetScale(glm::vec3 newScale);
	void SetPosition(glm::vec3 newPos);

	void SetRotation(glm::vec3 newRotation);

	void Translation(glm::vec3 direction);

	void Begin() override;

	void Tick(float DeltaTime) override;
};