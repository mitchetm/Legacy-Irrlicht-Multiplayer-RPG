#include "LockOnComponent.h"

LockOnComponent::LockOnComponent()
{
}

LockOnComponent::~LockOnComponent()
{
}

LockOnComponent::LockOnComponent(unsigned int id) : GameComponent(LOCK_ON_COMP, id)
{
	target = 0;
	hunterList.clear();
}

void LockOnComponent::setTarget(unsigned int value)
{
	target = value;
}

unsigned int LockOnComponent::getTarget(void)
{
	return target;
}

void LockOnComponent::addHunter(unsigned int value)
{
	if(!doesHunterExists(value))
		hunterList.push_back(value);
}

void LockOnComponent::removeHunter(unsigned int value)
{
	if(doesHunterExists(value))
	{
		int listSize = hunterList.size();

		for(int i = 0; i < listSize; i++)
		{
			if(hunterList[i] == value)
			{
				hunterList.erase(hunterList.begin() + i);
				return;
			}
		}
	}
}

void LockOnComponent::clearHunterList(void)
{
	hunterList.clear();
}

bool LockOnComponent::doesHunterExists(unsigned int value)
{
	bool isThere = false;

	int listSize = hunterList.size();

	for(int i = 0; i < listSize; i++)
	{
		if(hunterList[i] == value)
			isThere = true;
	}

	return isThere;
}

int LockOnComponent::getHunterListSize(void)
{
	return hunterList.size();
}

unsigned int LockOnComponent::getHunterByIndex(int index)
{
	return hunterList[index];
}
