#include "MoveTowardComponent.h"

MoveTowardComponent::MoveTowardComponent()
{
}

MoveTowardComponent::~MoveTowardComponent()
{
}

MoveTowardComponent::MoveTowardComponent(unsigned int id) : GameComponent(MOVE_TOWARD_COMP, id)
{
    x = 0;
    y = 0;
    z = 0;
}

void MoveTowardComponent::setMoveTowardX(float value)
{
    x = value;
}

float MoveTowardComponent::getMoveTowardX(void)
{
    return x;
}

void MoveTowardComponent::setMoveTowardY(float value)
{
    y = value;
}

float MoveTowardComponent::getMoveTowardY(void)
{
    return y;
}

void MoveTowardComponent::setMoveTowardZ(float value)
{
    z = value;
}

float MoveTowardComponent::getMoveTowardZ(void)
{
    return z;
}
