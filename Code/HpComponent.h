#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

class HpComponent : public GameComponent
{

public:

	HpComponent();
	~HpComponent();

	HpComponent(unsigned int id);

	void setMaxHP(int value);
	int getMaxHP(void);
	void setMinHP(int value);
	int getMinHP(void);
	void setHP(int value); // check if its between Max and Min
	int getHP(void);

private:

	int HP;
	int MaxHP;
	int MinHP;
};
