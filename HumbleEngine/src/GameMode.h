#pragma once
class GameMode {

public:
	virtual void BeginPlay();

	virtual void PreRenderer();

	virtual void Tick(float DeltaTime);

	virtual void PostRenderer();

 };