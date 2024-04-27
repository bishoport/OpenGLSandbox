#pragma once 
#include <string>
#include <random>
#include "ActorComponent.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"

class Actor {
public:
	std::string ActorName;
	Actor* parent;
	unsigned ActorID;
	

public:
	Actor(std::string Name = "");
	ActorComponent& AddComponent(Ref<ActorComponent> Component);
	template<typename T> T& GetComponent();

	template<typename T> bool ComponentExist();
	std::vector<Ref<ActorComponent>> ComponentList;

	Ref<TransformComponent> transform;

	virtual void Initialize(std::string Name = nullptr, unsigned number = 0);

	virtual void PreRenderer();

	virtual void PostRenderer();

	virtual void Tick(float deltaTime);

	virtual void OnColliderEnter(ColliderComponent* other);

};
