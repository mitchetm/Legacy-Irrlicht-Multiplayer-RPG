#include "MpComponent.h"

MpComponent::MpComponent()
{
}

MpComponent::~MpComponent()
{
}

MpComponent::MpComponent(unsigned int id) : GameComponent(MP_COMP, id)
{
	MP = 0;
	MaxMP = 0;
	MinMP = 0;
}

void MpComponent::setMaxMP(int value)
{
    MaxMP = value;
}

int MpComponent::getMaxMP(void)
{
    return MaxMP;
}

void MpComponent::setMinMP(int value)
{
    MinMP = value;
}

int MpComponent::getMinMP(void)
{
    return MinMP;
}

void MpComponent::setMP(int value)
{
    MP = value;
    
    if(MP > MaxMP)
        MP  = MaxMP;
    else if(MP < MinMP)
        MP = MinMP;
}

int MpComponent::getMP(void)
{
    return MP;
}
