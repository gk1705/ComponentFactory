#pragma once

#include <string>
#include <vector>

#include "IComponent.h"

class GameObject
{
public:
	std::string name = "Unnamed";
	std::vector<IComponent*> components;

	GameObject() = default;
	GameObject(std::string name) : name(std::move(name)) {}
	~GameObject();

	void AddComponent(IComponent* component);
	void RemoveComponent(const IComponent* component);

	template<typename T>
	T* GetComponent()
	{
		for (IComponent* component : components)
		{
			if (dynamic_cast<T*>(component) != nullptr)
			{
				return dynamic_cast<T*>(component);
			}
		}
		return nullptr;
	}

	void Init() const;
	void Update() const;
};