#pragma once

#include "ComponentType.h"

#include <map>
#include <string>

class GameObject
{

public:

	GameObject();
	~GameObject();

	GameObject(unsigned int id);

	void setID(unsigned int id);
	unsigned int getID(void);

	void setState(unsigned int state);
	unsigned int getState(void);

	void setPosition(float x, float y, float z);
	void getPosition(float &x, float &y, float &z);
	void setRotation(float x, float y, float z);
	void getRotation(float &x, float &y, float &z);
	void setScale(float x, float y, float z);
	void getScale(float &x, float &y, float &z);

	void addComponent(ComponentType type, unsigned int id);
	unsigned int getComponent(ComponentType type);

	unsigned int componentListSize(void); // how many GameComponents do we have
	ComponentType getComponentByIndex(unsigned int index); // example, get the 4th component in the list

	bool doComponentExists(ComponentType type);
	void removeComponent(ComponentType type);
	
	void addResource(std::string name, unsigned int id);
	unsigned int getResource(std::string name);

	unsigned int resourceListSize(void); // how many resources do we have
	std::string getResourceByIndex(unsigned int index); // example, get the 4th resource in the list

	bool doResourceExists(std::string name);
	void removeResource(std::string name);

private:

	unsigned int ID; // id of the object
	unsigned int State; // the state of the object

	float tx, ty, tz; // position
	float rx, ry, rz; // rotation
	float sx, sy, sz; // scale

	std::map<ComponentType, unsigned int> compList; // we store components by type
	std::map<std::string, unsigned int> resourceList; // we store resources by name

};
