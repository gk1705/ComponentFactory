#pragma once

class IComponent
{
public:
	virtual ~IComponent() = default;
	virtual void Update() = 0;
	virtual void Init() = 0;
};