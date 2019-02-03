#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

#include <string>

class ActionComponent : public GameComponent
{

public:

	ActionComponent();
	~ActionComponent();

	ActionComponent(unsigned int id);

	void setActionID(unsigned int value);
	unsigned int getActionID(void);

	void setActionPower(float value);
	float getActionPower(void);

private:

	unsigned int id;
	float power;
};
