#pragma once

#include "ComponentType.h"

class GameComponent
{

public:

	GameComponent();
	virtual ~GameComponent();

	GameComponent(ComponentType type, unsigned int id);

	void setType(ComponentType type);
	ComponentType getType(void);

	void setID(unsigned int id);
	unsigned int getID(void);

protected:


	unsigned int cid;
	ComponentType ctype;

};
