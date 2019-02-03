#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

class MpComponent : public GameComponent
{

public:

	MpComponent();
	~MpComponent();

	MpComponent(unsigned int id);

	void setMaxMP(int value);
	int getMaxMP(void);
	void setMinMP(int value);
	int getMinMP(void);
	void setMP(int value); // check if its between Max and Min
	int getMP(void);

private:

	int MP;
	int MaxMP;
	int MinMP;
};
