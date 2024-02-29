#pragma once

#include <iostream>

#include "IComponent.h"
#include "IMetaInfoProvider.h"
#include "Vector2.h"

class RigidbodyComponent : public IComponent, public IMetaInfoProvider
{
public:
	Vector2 velocity;
	float mass;

	void Update() override
	{
		std::cout << "RigidbodyComponent::Update()" << std::endl;
	}

	void Init() override
	{
		std::cout << "RigidbodyComponent::Init()" << std::endl;
	}

	std::vector<MetaInfo> GetMetaInfo() override
	{
		return {
			{ STRINGIFY(velocity), typeid(Vector2).name(), offsetof(RigidbodyComponent, velocity) },
			{ STRINGIFY(mass), typeid(float).name(), offsetof(RigidbodyComponent, mass) }
		};
	}
};
