#include "GameObject.h"

GameObject::~GameObject()
{
	for (const IComponent* component : components)
	{
		delete component;
	}
}

void GameObject::AddComponent(IComponent* component)
{
	components.push_back(component);
}

void GameObject::RemoveComponent(const IComponent* component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == component)
		{
			components.erase(components.begin() + i);
			return;
		}
	}
}

void GameObject::Init() const
{
	for (IComponent* component : components)
	{
		component->Init();
	}
}

void GameObject::Update() const
{
	for (IComponent* component : components)
	{
		component->Update();
	}
}