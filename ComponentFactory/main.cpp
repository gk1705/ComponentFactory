#include <iostream>
#include <vector>
#include <typeinfo>
#include <string>
#include <unordered_map>
#include <concepts>

#include "tinyxml2.h"
#include "Vector2.h"

#include "IComponentFactory.h"
#include "ComponentFactory.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "StateMachineComponent.h"
#include "AnimationComponent.h"
#include "RigidbodyComponent.h"

#include "XMLFactoryUtility.h"

void Test()
{
	std::unordered_map<std::string, IComponentFactory*> factories;
	REGISTER_FACTORY(TransformComponent);
	REGISTER_FACTORY(StateMachineComponent);
	REGISTER_FACTORY(AnimationComponent);
	REGISTER_FACTORY(RigidbodyComponent);

	using xfu = ::XMLFactoryUtility;
	const std::vector<GameObject*> gameObjects = xfu::createGameObjectsFromXMLDoc("gameobjects.xml", factories);
	const auto obj = gameObjects[0];
	obj->GetComponent<TransformComponent>()->position.x = 77.0f;
	obj->GetComponent<TransformComponent>()->rotation = 44.7f;
	xfu::saveGameObjectsToXML(gameObjects, "gameobjectsaltered.xml");
	for (const auto gameObject : gameObjects)
	{
		delete gameObject;
	}

	const std::vector<GameObject*> gameObjects2 = xfu::createGameObjectsFromXMLDoc("gameobjectsaltered.xml", factories);
	const auto obj2 = gameObjects2[0];
	obj2->name = "Altered";
	obj2->GetComponent<StateMachineComponent>()->states.emplace_back("Flying");
	xfu::saveGameObjectsToXML(gameObjects2, "gameobjectsaltered2.xml");

	for (const auto gameObject : gameObjects2)
	{
		delete gameObject;
	}

	for (const auto factory : factories)
	{
		delete factory.second;
	}
}

int main()
{
	Test();
	std::cout << "Program finished executing." << std::endl;

    return 0;
}