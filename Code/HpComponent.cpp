#include "HpComponent.h"

HpComponent::HpComponent()
{
}

HpComponent::~HpComponent()
{
}

HpComponent::HpComponent(unsigned int id) : GameComponent(HP_COMP, id)
{
	HP = 0;
	MaxHP = 0;
	MinHP = 0;
}

void HpComponent::setMaxHP(int value)
{
    MaxHP = value;
}

int HpComponent::getMaxHP(void)
{
    return MaxHP;
}

void HpComponent::setMinHP(int value)
{
    MinHP = value;
}

int HpComponent::getMinHP(void)
{
    return MinHP;
}

void HpComponent::setHP(int value)
{
    HP = value;
    
    if(HP > MaxHP)
        HP  = MaxHP;
    else if(HP < MinHP)
        HP = MinHP;
}

int HpComponent::getHP(void)
{
    return HP;
}
