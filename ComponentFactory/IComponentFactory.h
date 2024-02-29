#pragma once

#include "IComponent.h"

template<typename T>
concept IsIComponentDerived = std::is_base_of<IComponent, T>::value;

class IComponentFactory
{
public:
	virtual ~IComponentFactory() {}
	virtual IComponent* CreateComponent(const std::vector<std::pair<std::string, std::string>>& arguments) = 0;
};