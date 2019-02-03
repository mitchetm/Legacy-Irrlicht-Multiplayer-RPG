#include "GameComponent.h"

GameComponent::GameComponent()
{
}

GameComponent::~GameComponent()
{
}

GameComponent::GameComponent(ComponentType type, unsigned int id)
{
	ctype = type;
	cid = id;
}

void GameComponent::setType(ComponentType type)
{
	ctype = type;
}

ComponentType GameComponent::getType(void)
{
	return ctype;
}

void GameComponent::setID(unsigned int id)
{
	cid = id;
}

unsigned int GameComponent::getID(void)
{
	return cid;
}
