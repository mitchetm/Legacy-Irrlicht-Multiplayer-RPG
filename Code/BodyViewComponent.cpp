#include "BodyViewComponent.h"

BodyViewComponent::BodyViewComponent()
{
}

BodyViewComponent::~BodyViewComponent()
{
}

BodyViewComponent::BodyViewComponent(unsigned int id) : GameComponent(BODY_VIEW_COMP, id)
{
    bodyID = 0;
	yOffset = 0;
	rotationOffset = 0;
    red = 255;
    green = 255;
    blue = 255;
}

void BodyViewComponent::setBodyViewID(unsigned int value)
{
    bodyID = value;
}

unsigned int BodyViewComponent::getBodyViewID(void)
{
    return bodyID;
}

void BodyViewComponent::setYOffset(float value)
{
	yOffset = value;
}

float BodyViewComponent::getYOffset(void)
{
	return yOffset;
}

void BodyViewComponent::setRotationOffset(float value)
{
	rotationOffset = value;
}

float BodyViewComponent::getRotationOffset(void)
{
	return rotationOffset;
}

void BodyViewComponent::setRed(unsigned int value)
{
     red = value;
}

unsigned int BodyViewComponent::getRed(void)
{
    return red;
}

void BodyViewComponent::setGreen(unsigned int value)
{
     green = value;
}

unsigned int BodyViewComponent::getGreen(void)
{
    return green;
}

void BodyViewComponent::setBlue(unsigned int value)
{
     blue = value;
}

unsigned int BodyViewComponent::getBlue(void)
{
    return blue;
}
