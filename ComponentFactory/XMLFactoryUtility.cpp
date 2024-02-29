#include "XMLFactoryUtility.h"

#include <iostream>
#include <ostream>

#include "IMetaInfoProvider.h"
#include "Vector2.h"

void XMLFactoryUtility::printElement(const tinyxml2::XMLElement* element, int depth)
{
	if (element == nullptr)
	{
		std::cout << "Element is null" << std::endl;
		return;
	}

	for (int i = 0; i < depth; i++)
	{
		std::cout << "  ";
	}

	std::cout << element->Name();
	if (element->GetText() != nullptr)
	{
		std::cout << ": " << element->GetText();
	}
	std::cout << std::endl;

	const tinyxml2::XMLElement* child = element->FirstChildElement();
	while (child != nullptr)
	{
		printElement(child, depth + 1);
		child = child->NextSiblingElement();
	}
}

std::vector<GameObject*> XMLFactoryUtility::createGameObjectsFromXMLDoc(std::string filePath,
	std::unordered_map<std::string, IComponentFactory*> factories)
{
	std::vector<GameObject*> gameObjects;

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError result = doc.LoadFile(filePath.c_str());
	if (result != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Error loading file: " << result << std::endl;
		return gameObjects;
	}

	tinyxml2::XMLElement* currentGameObject = doc.RootElement()->FirstChildElement();
	while (currentGameObject != nullptr)
	{
		if (strcmp(currentGameObject->Name(), "GameObject") != 0)
		{
			std::cout << "Error: Element is not GameObject" << std::endl;
			return gameObjects;
		}
		GameObject* gameObject = new GameObject();

		tinyxml2::XMLElement* child = currentGameObject->FirstChildElement();
		while (child != nullptr)
		{
			if (strcmp(child->Name(), "name") == 0)
			{
				gameObject->name = child->GetText();
			}
			else
			{
				std::vector<std::pair<std::string, std::string>> arguments;
				tinyxml2::XMLElement* argument = child->FirstChildElement();
				while (argument != nullptr)
				{
					arguments.emplace_back(argument->Name(), argument->GetText());
					argument = argument->NextSiblingElement();
				}

				const char* componentName = child->Name();
				auto factoryIter = factories.find(componentName);
				if (factoryIter != factories.end())
				{
					auto factory = factoryIter->second;
					if (factory != nullptr)
					{
						IComponent* component = factory->CreateComponent(arguments);
						gameObject->AddComponent(component);
					}
					else
					{
						std::cout << "Error: Component factory is not instantiated for: " << componentName << std::endl;
					}
				}
				else
				{
					std::cout << "Error: Component factory not registered for: " << componentName << std::endl;
				}
			}
			child = child->NextSiblingElement();
		}

		gameObjects.push_back(gameObject);
		currentGameObject = currentGameObject->NextSiblingElement();
	}

	return gameObjects;
}

void XMLFactoryUtility::saveGameObjectsToXML(const std::vector<GameObject*>& gameObjects, const std::string& filePath)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLElement* root = doc.NewElement("GameObjects");
	doc.InsertFirstChild(root);

	for (const auto& gameObject : gameObjects)
	{
		tinyxml2::XMLElement* gameObjectElement = doc.NewElement("GameObject");
		root->InsertEndChild(gameObjectElement);

		tinyxml2::XMLElement* nameElement = doc.NewElement("name");
		nameElement->SetText(gameObject->name.c_str());
		gameObjectElement->InsertEndChild(nameElement);

		for (const auto& component : gameObject->components)
		{
			std::string componentName = typeid(*component).name();
			componentName = componentName.substr(6, componentName.size() - 6); //remove "class "

			tinyxml2::XMLElement* componentElement = doc.NewElement(componentName.c_str());
			gameObjectElement->InsertEndChild(componentElement);

			IMetaInfoProvider* metaInfoProvider = dynamic_cast<IMetaInfoProvider*>(component);
			if (metaInfoProvider == nullptr)
			{
				std::cout << "Error: Component does not implement IMetaInfoProvider" << std::endl;
				continue;
			}

			std::vector<MetaInfo> memberMetaInfo = metaInfoProvider->GetMetaInfo();
			for (const auto& member : memberMetaInfo)
			{
				tinyxml2::XMLElement* memberElement = doc.NewElement(member.name);
				componentElement->InsertEndChild(memberElement);

				char* ptr = reinterpret_cast<char*>(component) + member.offset;
				if (strcmp(member.type, typeid(int).name()) == 0)
				{
					memberElement->SetText(*reinterpret_cast<int*>(ptr));
				}
				else if (strcmp(member.type, typeid(float).name()) == 0)
				{
					memberElement->SetText(*reinterpret_cast<float*>(ptr));
				}
				else if (strcmp(member.type, typeid(Vector2).name()) == 0)
				{
					Vector2 vector = *reinterpret_cast<Vector2*>(ptr);
					memberElement->SetText((std::to_string(vector.x) + "," + std::to_string(vector.y)).c_str());
				}
				else if (strcmp(member.type, typeid(std::string).name()) == 0)
				{
					memberElement->SetText(reinterpret_cast<std::string*>(ptr)->c_str());
				}
				else if (strcmp(member.type, typeid(std::vector<std::string>).name()) == 0)
				{
					std::vector<std::string> states = *reinterpret_cast<std::vector<std::string>*>(ptr);
					std::string statesString;
					for (int i = 0; i < states.size(); i++)
					{
						statesString += states[i];
						if (i < states.size() - 1)
						{
							statesString += ",";
						}
					}
					memberElement->SetText(statesString.c_str());
				}
				else
				{
					std::cout << "Unknown type: " << member.type << std::endl;
				}
			}
		}
	}

	tinyxml2::XMLError result = doc.SaveFile(filePath.c_str());
	if (result != tinyxml2::XML_SUCCESS)
	{
		std::cout << "Error saving the file: " << result << std::endl;
	}
}

