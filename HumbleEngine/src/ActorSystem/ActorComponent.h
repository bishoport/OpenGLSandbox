#pragma once
class Actor;
class ActorComponent {
public:
	int IDActor;
	Actor* actor;
	virtual void Begin() = 0;


	virtual void PreTick(float DeltaTime);
	virtual void Tick(float DeltaTime) = 0;
	bool rendererBool;
};