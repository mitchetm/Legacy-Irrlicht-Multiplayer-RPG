#pragma once

#include "ComponentType.h"
#include "GameComponent.h"


// The BodyViewComponent contains information of what the characters model and color is
// This will change when we get better body and model formation

class BodyViewComponent : public GameComponent
{

public:

	BodyViewComponent();
	~BodyViewComponent();

	BodyViewComponent(unsigned int id);
	
	void setBodyViewID(unsigned int value);
	unsigned int getBodyViewID(void);
	void setYOffset(float value);
	float getYOffset(void);
	void setRotationOffset(float value);
	float getRotationOffset(void);
	void setRed(unsigned int value);
	unsigned int getRed(void);
	void setGreen(unsigned int value);
	unsigned int getGreen(void);
	void setBlue(unsigned int value);
	unsigned int getBlue(void);

private:
        
    unsigned int bodyID;
	float yOffset;
	float rotationOffset;
    unsigned int red;
    unsigned int green;
    unsigned int blue;

};
