#include "ApComponent.h"

ApComponent::ApComponent()
{
}

ApComponent::~ApComponent()
{
}

ApComponent::ApComponent(unsigned int id) : GameComponent(AP_COMP, id)
{
	AP = 0;
	MaxAP = 0;
	MinAP = 0;
}

void ApComponent::setMaxAP(int value)
{
    MaxAP = value;
}

int ApComponent::getMaxAP(void)
{
    return MaxAP;
}

void ApComponent::setMinAP(int value)
{
    MinAP = value;
}

int ApComponent::getMinAP(void)
{
    return MinAP;
}

void ApComponent::setAP(int value)
{
    AP = value;
    
    if(AP > MaxAP)
        AP  = MaxAP;
    else if(AP < MinAP)
        AP = MinAP;
}

int ApComponent::getAP(void)
{
    return AP;
}
