#pragma once
#include <iostream>
#include <string>

#include "IComponent.h"
#include "IMetaInfoProvider.h"

class StateMachineComponent : public IComponent, public IMetaInfoProvider
{
public:
	std::vector<std::string> states;

	void Update() override
	{
		std::cout << "StateMachineComponent::Update()" << std::endl;
	}

	void Init() override
	{
		std::cout << "StateMachineComponent::Init()" << std::endl;
	}

	std::vector<MetaInfo> GetMetaInfo() override
	{
		return {
			{ STRINGIFY(states), typeid(std::vector<std::string>).name(), offsetof(StateMachineComponent, states) }
		};
	}
};
