#pragma once
#include <vector>
#include "ActorSystem/Actor.h"
class GameMode {

public:

	std::vector<Ref<Actor>> scenesActors;

	virtual void BeginPlay();

	virtual void PreRenderer();

	virtual void Tick(float DeltaTime);

	virtual void PostRenderer();

 };