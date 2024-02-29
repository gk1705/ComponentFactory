#pragma once

#include <iostream>
#include <string>

#include "IComponent.h"
#include "IMetaInfoProvider.h"

class AnimationComponent : public IComponent, public IMetaInfoProvider
{
public:
	std::string path;
	int rows, columns;

	void Update() override
	{
		std::cout << "AnimationComponent::Update()" << std::endl;
	}

	void Init() override
	{
		std::cout << "AnimationComponent::Init()" << std::endl;
	}

	std::vector<MetaInfo> GetMetaInfo() override
	{
		return {
			{ STRINGIFY(path), typeid(std::string).name(), offsetof(AnimationComponent, path) },
			{ STRINGIFY(rows), typeid(int).name(), offsetof(AnimationComponent, rows) },
			{ STRINGIFY(columns), typeid(int).name(), offsetof(AnimationComponent, columns) }
		};
	}
};