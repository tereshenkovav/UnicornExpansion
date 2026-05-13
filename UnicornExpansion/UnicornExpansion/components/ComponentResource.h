#pragma once
#include "UnitComponent.h"

enum class ResourceType { Poor, Norm, Rich };

// Компонент ресурса - его можно добывать
class ComponentResource: public UnitComponent
{
private:
	float efficient;
	std::string info;
public:
	ComponentResource(Game * game, ResourceType rt = ResourceType::Norm);
	float getResourceEfficient() const;
	virtual std::string getComponentInfo() const;
};

