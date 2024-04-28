#pragma once
#include "../ActorComponent.h"
#include <LibCore.h>
#include "TransformComponent.h"
#include <tools/Camera.h>

class RendererComponent : public ActorComponent {
public:
	Ref<Mesh> CurrentMesh;
	Ref<Texture> mainTexture;
	
	Ref<libCore::Shader> CurrentShader;
	Ref<libCore::Camera> CurrentCamera;
	Ref<TransformComponent> Transform;

	RendererComponent();

	glm::mat4 projection();

	glm::mat4 view();

	glm::mat4 model();

	void Begin() override;

	void Tick(float deltaTime) override;

	void RendererProcess(bool isLightingOnlyBool);
};