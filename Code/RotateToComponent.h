#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

// we use this component to mark the next direction the object is suppose to look at

class RotateToComponent : public GameComponent
{

public:

	RotateToComponent();
	~RotateToComponent();

	RotateToComponent(unsigned int id);

	void setRotateToX(float value);
	float getRotateToX(void);
	void setRotateToY(float value);
	float getRotateToY(void);
	void setRotateToZ(float value);
	float getRotateToZ(void);

private:

	float x;
	float y;
	float z;
};
