#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

// we use this component to remeber which direciton we were moving toward

class MoveTowardComponent : public GameComponent
{

public:

	MoveTowardComponent();
	~MoveTowardComponent();

	MoveTowardComponent(unsigned int id);

	void setMoveTowardX(float value);
	float getMoveTowardX(void);
	void setMoveTowardY(float value);
	float getMoveTowardY(void);
	void setMoveTowardZ(float value);
	float getMoveTowardZ(void);

private:

	float x;
	float y;
	float z;
};
