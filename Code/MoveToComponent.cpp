#include "MoveToComponent.h"

MoveToComponent::MoveToComponent()
{
}

MoveToComponent::~MoveToComponent()
{
}

MoveToComponent::MoveToComponent(unsigned int id) : GameComponent(MOVE_TO_COMP, id)
{
    x = 0;
    y = 0;
    z = 0;
}

void MoveToComponent::setMoveToX(float value)
{
    x = value;
}

float MoveToComponent::getMoveToX(void)
{
    return x;
}

void MoveToComponent::setMoveToY(float value)
{
    y = value;
}

float MoveToComponent::getMoveToY(void)
{
    return y;
}

void MoveToComponent::setMoveToZ(float value)
{
    z = value;
}

float MoveToComponent::getMoveToZ(void)
{
    return z;
}
