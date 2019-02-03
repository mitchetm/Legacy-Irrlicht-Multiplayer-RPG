#include "RotationComponent.h"

RotationComponent::RotationComponent()
{
}

RotationComponent::~RotationComponent()
{
}

RotationComponent::RotationComponent(unsigned int id) : GameComponent(ROTATION_COMP, id)
{
    sx = 0;
    sy = 0;
    sz = 0;
    ax = 0;
    ay = 0;
    az = 0;
}

void RotationComponent::setRotationSpeedX(float value)
{
    sx = value;
}

float RotationComponent::getRotationSpeedX(void)
{
    return sx;
}

void RotationComponent::setRotationSpeedY(float value)
{
    sy = value;
}

float RotationComponent::getRotationSpeedY(void)
{
    return sy;
}

void RotationComponent::setRotationSpeedZ(float value)
{
    sz = value;
}

float RotationComponent::getRotationSpeedZ(void)
{
    return sz;
}

void RotationComponent::setRotationAccelerationX(float value)
{
    ax = value;
}

float RotationComponent::getRotationAccelerationX(void)
{
    return ax;
}

void RotationComponent::setRotationAccelerationY(float value)
{
    ay = value;
}

float RotationComponent::getRotationAccelerationY(void)
{
    return ay;
}

void RotationComponent::setRotationAccelerationZ(float value)
{
    az = value;
}

float RotationComponent::getRotationAccelerationZ(void)
{
    return az;
}
