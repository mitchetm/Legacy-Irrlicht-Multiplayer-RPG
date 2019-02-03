#include "ActionComponent.h"

ActionComponent::ActionComponent()
{
}

ActionComponent::~ActionComponent()
{
}

ActionComponent::ActionComponent(unsigned int id) : GameComponent(ACTION_COMP, id)
{
	id = 0;
	power = 0.0;
}

void ActionComponent::setActionID(unsigned int value)
{
	id = value;
}

unsigned int ActionComponent::getActionID(void)
{
	return id;
}

void ActionComponent::setActionPower(float value)
{
	power = value;
}

float ActionComponent::getActionPower(void)
{
	return power;
}
