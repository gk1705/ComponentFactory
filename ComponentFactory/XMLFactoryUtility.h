#pragma once

#include <unordered_map>
#include <vector>

#include "tinyxml2.h"
#include "GameObject.h"
#include "IComponentFactory.h"

class XMLFactoryUtility
{
public:
	XMLFactoryUtility() = delete;
	static void printElement(const tinyxml2::XMLElement* element, int depth = 0);
	static std::vector<GameObject*> createGameObjectsFromXMLDoc(std::string filePath, std::unordered_map<std::string, IComponentFactory*> factories);
	static void saveGameObjectsToXML(const std::vector<GameObject*>& gameObjects, const std::string& filePath);
};
