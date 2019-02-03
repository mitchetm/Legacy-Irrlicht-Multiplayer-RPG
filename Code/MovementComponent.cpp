#include "MovementComponent.h"

MovementComponent::MovementComponent()
{
}

MovementComponent::~MovementComponent()
{
}

MovementComponent::MovementComponent(unsigned int id) : GameComponent(MOVEMENT_COMP, id)
{
    sx = 0;
    sy = 0;
    sz = 0;
    ax = 0;
    ay = 0;
    az = 0;
}

void MovementComponent::setSpeedX(float value)
{
    sx = value;
}

float MovementComponent::getSpeedX(void)
{
    return sx;
}

void MovementComponent::setSpeedY(float value)
{
    sy = value;
}

float MovementComponent::getSpeedY(void)
{
    return sy;
}

void MovementComponent::setSpeedZ(float value)
{
    sz = value;
}

float MovementComponent::getSpeedZ(void)
{
    return sz;
}

void MovementComponent::setAccelerationX(float value)
{
    ax = value;
}

float MovementComponent::getAccelerationX(void)
{
    return ax;
}

void MovementComponent::setAccelerationY(float value)
{
    ay = value;
}

float MovementComponent::getAccelerationY(void)
{
    return ay;
}

void MovementComponent::setAccelerationZ(float value)
{
    az = value;
}

float MovementComponent::getAccelerationZ(void)
{
    return az;
}
