#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

class ApComponent : public GameComponent
{

public:

	ApComponent();
	~ApComponent();

	ApComponent(unsigned int id);

	void setMaxAP(int value);
	int getMaxAP(void);
	void setMinAP(int value);
	int getMinAP(void);
	void setAP(int value); // check if its between Max and Min
	int getAP(void);

private:

	int AP;
	int MaxAP;
	int MinAP;
};
