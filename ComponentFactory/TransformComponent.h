#pragma once

#include <iostream>

#include "IComponent.h"
#include "IMetaInfoProvider.h"
#include "Vector2.h"

class TransformComponent : public IComponent, public IMetaInfoProvider
{
public:
	Vector2 position;
	Vector2 scale;
	float rotation;

	void Update() override
	{
		std::cout << "TransformComponent::Update()" << std::endl;
	}

	void Init() override
	{
		std::cout << "TransformComponent::Init()" << std::endl;
	}

	std::vector<MetaInfo> GetMetaInfo() override
	{
		return {
			{ STRINGIFY(position), typeid(Vector2).name(), offsetof(TransformComponent, position) },
			{ STRINGIFY(scale), typeid(Vector2).name(), offsetof(TransformComponent, scale) },
			{ STRINGIFY(rotation), typeid(float).name(), offsetof(TransformComponent, rotation) }
		};
	}
};
