#include "RotateToComponent.h"

RotateToComponent::RotateToComponent()
{
}

RotateToComponent::~RotateToComponent()
{
}

RotateToComponent::RotateToComponent(unsigned int id) : GameComponent(ROTATE_TO_COMP, id)
{
    x = 0;
    y = 0;
    z = 0;
}

void RotateToComponent::setRotateToX(float value)
{
    x = value;
}

float RotateToComponent::getRotateToX(void)
{
    return x;
}

void RotateToComponent::setRotateToY(float value)
{
    y = value;
}

float RotateToComponent::getRotateToY(void)
{
    return y;
}

void RotateToComponent::setRotateToZ(float value)
{
    z = value;
}

float RotateToComponent::getRotateToZ(void)
{
    return z;
}
