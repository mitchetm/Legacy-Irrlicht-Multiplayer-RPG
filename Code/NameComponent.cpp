#include "NameComponent.h"

NameComponent::NameComponent()
{
}

NameComponent::~NameComponent()
{
}

NameComponent::NameComponent(unsigned int id) : GameComponent(NAME_COMP, id)
{
}

void NameComponent::setName(std::string value)
{
    name = value;
}

std::string NameComponent::getName(void)
{
    return name;
}
