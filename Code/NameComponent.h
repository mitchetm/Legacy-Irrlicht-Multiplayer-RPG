#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

#include <string>

class NameComponent : public GameComponent
{

public:

	NameComponent();
	~NameComponent();

	NameComponent(unsigned int id);

	void setName(std::string value);
	std::string getName(void);

private:

	std::string name;
};
