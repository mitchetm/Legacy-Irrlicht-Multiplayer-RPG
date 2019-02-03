#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

#include <vector>

class LockOnComponent : public GameComponent
{

public:

	LockOnComponent();
	~LockOnComponent();

	LockOnComponent(unsigned int id);

	void setTarget(unsigned int value);
	unsigned int getTarget(void);

	void addHunter(unsigned int value);
	void removeHunter(unsigned int value);
	void clearHunterList(void);

	bool doesHunterExists(unsigned int value);
	int getHunterListSize(void);
	unsigned int getHunterByIndex(int index);

private:

	unsigned int target; // who I am locked on to
	std::vector<unsigned int> hunterList; // who is/are locked on to me
};
