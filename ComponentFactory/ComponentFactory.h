#pragma once

#include "MetaInfo.h"
#include "IComponent.h"

#define REGISTER_FACTORY(classname) \
    factories[STRINGIFY(classname)] = new ComponentFactory<classname>();

template<IsIComponentDerived T>
class ComponentFactory : public IComponentFactory
{
public:
	T* Create(const std::vector<std::pair<std::string, std::string>>& arguments)
	{
		T* obj = new T();

		const std::vector<MetaInfo> memberMetaInfo = obj->GetMetaInfo();
		if (memberMetaInfo.size() != arguments.size())
		{
			std::cout << "Error: Number of arguments does not match number of members" << std::endl;
			return nullptr;
		}
		for (int i = 0; i < memberMetaInfo.size(); i++)
		{
			if (memberMetaInfo[i].name != arguments[i].first)
			{
				std::cout << "Error: Argument name does not match member name" << std::endl;
				std::cout << "Expected for " << typeid(T).name() << ":" << std::endl;
				for (int j = 0; j < memberMetaInfo.size(); j++)
				{
					std::cout << memberMetaInfo[j].name << " " << arguments[j].first << std::endl;
				}
				return nullptr;
			}
		}

		for (int i = 0; i < arguments.size(); i++)
		{
			char* ptr = reinterpret_cast<char*>(obj) + memberMetaInfo[i].offset;
			if (strcmp(memberMetaInfo[i].type, typeid(int).name()) == 0)
			{
				*reinterpret_cast<int*>(ptr) = std::stoi(arguments[i].second);
			}
			else if (strcmp(memberMetaInfo[i].type, typeid(float).name()) == 0)
			{
				*reinterpret_cast<float*>(ptr) = std::stof(arguments[i].second);
			}
			else if (strcmp(memberMetaInfo[i].type, typeid(Vector2).name()) == 0)
			{
				std::vector<std::string> vectorComponents = split(arguments[i].second, ",");
				if (vectorComponents.size() != 2)
				{
					std::cout << "Error: Vector2 must have 2 components" << std::endl;
					return nullptr;
				}
				*reinterpret_cast<Vector2*>(ptr) = Vector2{ std::stof(vectorComponents[0]), std::stof(vectorComponents[1]) };
			}
			else if (strcmp(memberMetaInfo[i].type, typeid(std::string).name()) == 0)
			{
				*reinterpret_cast<std::string*>(ptr) = arguments[i].second;
			}
			else if (strcmp(memberMetaInfo[i].type, typeid(std::vector<std::string>).name()) == 0)
			{
				const std::vector<std::string> states = split(arguments[i].second, ",");
				*reinterpret_cast<std::vector<std::string>*>(ptr) = states;
			}
			else
			{
				std::cout << "Unknown type: " << memberMetaInfo[i].type << std::endl;
			}
		}

		return obj;
	}

	IComponent* CreateComponent(const std::vector<std::pair<std::string, std::string>>& arguments) override
	{
		return Create(arguments);
	}

private:
	static std::vector<std::string> split(const std::string& s, const std::string& delimiter)
	{
		size_t posStart = 0, posEnd;
		const size_t delimLen = delimiter.length();
		std::vector<std::string> res;

		while ((posEnd = s.find(delimiter, posStart)) != std::string::npos)
		{
			std::string token = s.substr(posStart, posEnd - posStart);
			posStart = posEnd + delimLen;
			res.push_back(token);
		}

		res.push_back(s.substr(posStart));
		return res;
	}
};