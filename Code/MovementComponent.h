#pragma once

#include "ComponentType.h"
#include "GameComponent.h"

class MovementComponent : public GameComponent
{

public:

	MovementComponent();
	~MovementComponent();

	MovementComponent(unsigned int id);

	void setSpeedX(float value);
	float getSpeedX(void);
	void setSpeedY(float value);
	float getSpeedY(void);
	void setSpeedZ(float value);
	float getSpeedZ(void);
	void setAccelerationX(float value);
	float getAccelerationX(void);
	void setAccelerationY(float value);
	float getAccelerationY(void);
	void setAccelerationZ(float value);
	float getAccelerationZ(void);

private:

	float sx; // speed X
	float sy;
	float sz;
	float ax; // gravity/acceleration X 
	float ay;
	float az;
};
