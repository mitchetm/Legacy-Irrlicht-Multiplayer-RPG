#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

// we use this component to mark the next location the object is suppose to move to

class MoveToComponent : public GameComponent
{

public:

	MoveToComponent();
	~MoveToComponent();

	MoveToComponent(unsigned int id);

	void setMoveToX(float value);
	float getMoveToX(void);
	void setMoveToY(float value);
	float getMoveToY(void);
	void setMoveToZ(float value);
	float getMoveToZ(void);

private:

	float x;
	float y;
	float z;
};
