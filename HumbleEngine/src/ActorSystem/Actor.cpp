#include "Actor.h"



Actor::Actor(std::string Name)
{
	ActorName = Name;
	transform = std::make_shared<TransformComponent>();
	AddComponent(transform);
	std::random_device rd;
	std::mt19937 gen(rd());

	// Definir la distribuci�n para n�meros de 10 d�gitos
	std::uniform_int_distribution<int> distribution(1000000000, 9999999999);

	// Generar y mostrar el n�mero aleatorio
	int randomNumber = distribution(gen);
	ActorID = randomNumber;
}

ActorComponent& Actor::AddComponent(Ref<ActorComponent> Component)
{
	// TODO: Insertar una instrucci�n "return" aqu�
	if (Component->IDActor == 0)
		Component->IDActor = ActorID;
	auto iterador = std::find(ComponentList.begin(), ComponentList.end(), Component);
	if (iterador == ComponentList.end()) {
		ComponentList.emplace_back(Component);
		return *Component;
	}
	Ref<ActorComponent> newComponent = *iterador;
	return *newComponent;
}

void Actor::Initialize(std::string Name, unsigned number)
{
	if (Name.length() <= 0) {
		int value = number + 1;
		ActorName = "ACTOR_" + std::to_string(value);
	}
	else {
		ActorName = Name;
	}
}

void Actor::PreRenderer()
{
}

void Actor::PostRenderer()
{
}

void Actor::Tick(float deltaTime)
{
}

void Actor::OnColliderEnter(ColliderComponent* other)
{
}


template<typename T>
inline T& Actor::GetComponent()
{
	for (int i = 0; i < ComponentList.size(); i++) {
		Ref<ActorComponent> actorComp = ComponentList[i];
		if (dynamic_cast<T*>(actorComp) != nullptr) {
			return *dynamic_cast<T*>(ComponentList[i]);
		}
	}
	throw std::runtime_error("Component of type T not found");
	// TODO: Insertar una instrucci�n "return" aqu�
}

template<typename T>
bool Actor::ComponentExist()
{
	for (int i = 0; i < ComponentList.size(); i++) {
		Ref<ActorComponent> actorComp = ComponentList[i];
		if (dynamic_cast<T*>(actorComp) != nullptr)
			return true;

	}
	return false;
}
