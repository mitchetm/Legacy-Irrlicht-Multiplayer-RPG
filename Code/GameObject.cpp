#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

GameObject::GameObject(unsigned int id)
{
	ID = id;
	State = 0;
	tx = 0;
	ty = 0;
	tz = 0;
	rx = 0;
	ry = 0;
	rz = 0;
	sx = 1;
	sy = 1;
	sz = 1;
}

void GameObject::setID(unsigned int id)
{
	ID = id;
}

unsigned int GameObject::getID(void)
{
	return ID;
}

void GameObject::setState(unsigned int state)
{
	State = state;
}

unsigned int GameObject::getState(void)
{
	return State;
}

void GameObject::setPosition(float x, float y, float z)
{
	tx = x;
	ty = y;
	tz = z;
}

void GameObject::getPosition(float &x, float &y, float &z)
{
	x = tx;
	y = ty;
	z = tz;
}

void GameObject::setRotation(float x, float y, float z)
{
	rx = x;
	ry = y;
	rz = z;
}

void GameObject::getRotation(float &x, float &y, float &z)
{
	x = rx;
	y = ry;
	z = rz;
}

void GameObject::setScale(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
}

void GameObject::getScale(float &x, float &y, float &z)
{
	x = sx;
	y = sy;
	z = sz;
}

void GameObject::addComponent(ComponentType type, unsigned int id)
{
	if(!doComponentExists(type))
	{
		compList[type] = id;
	}
	else
	{
		// throw something
	}
}

unsigned int GameObject::getComponent(ComponentType type)
{
	if(doComponentExists(type))
	{
		return compList[type];
	}
	else
	{
		return 0;
	}
}

unsigned int GameObject::componentListSize(void)
{
	return compList.size();
}

ComponentType GameObject::getComponentByIndex(unsigned int index)
{
	std::map<ComponentType, unsigned int>::iterator itr;

	itr = compList.begin();
	//itr = itr + index;
	
	// rats, can't get it to work. Plan B
    for(int i = 0; i < index; i++)
    {
        itr++;
    }

	return itr->first;
}

bool GameObject::doComponentExists(ComponentType type)
{
	return (compList.find(type) != compList.end());
}

void GameObject::removeComponent(ComponentType type)
{
	if(doComponentExists(type))
	{
		compList.erase(type);
	}
	else
	{
		// throw something
	}
}

void GameObject::addResource(std::string name, unsigned int id)
{
	if(!doResourceExists(name))
	{
		resourceList[name] = id;
	}
	else
	{
		// throw something
	}
}

unsigned int GameObject::getResource(std::string name)
{
	if(doResourceExists(name))
	{
		return resourceList[name];
	}
	else
	{
		return 0;
	}
}

unsigned int GameObject::resourceListSize(void)
{
	return resourceList.size();
}

std::string GameObject::getResourceByIndex(unsigned int index)
{
	std::map<std::string, unsigned int>::iterator itr;

	itr = resourceList.begin();
	//itr += index;

    for(int i = 0; i < index; i++)
    {
        itr++;
    }

	return itr->first;
}

bool GameObject::doResourceExists(std::string name)
{
	return (resourceList.find(name) != resourceList.end());
}

void GameObject::removeResource(std::string name)
{
	if(doResourceExists(name))
	{
		resourceList.erase(name);
	}
	else
	{
		// throw something
	}
}
