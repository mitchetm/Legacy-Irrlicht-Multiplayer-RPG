#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

class RotationComponent : public GameComponent
{

public:

	RotationComponent();
	~RotationComponent();

	RotationComponent(unsigned int id);

	void setRotationSpeedX(float value);
	float getRotationSpeedX(void);
	void setRotationSpeedY(float value);
	float getRotationSpeedY(void);
	void setRotationSpeedZ(float value);
	float getRotationSpeedZ(void);
	void setRotationAccelerationX(float value);
	float getRotationAccelerationX(void);
	void setRotationAccelerationY(float value);
	float getRotationAccelerationY(void);
	void setRotationAccelerationZ(float value);
	float getRotationAccelerationZ(void);

private:

	float sx; // speed X
	float sy;
	float sz;
	float ax; // gravity/acceleration X 
	float ay;
	float az;
};
