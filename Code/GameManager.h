#pragma once

#include "Singleton.h"

#include "ComponentType.h"
#include "GameComponent.h"
#include "GameObject.h"

// we declare the components here
#include "NameComponent.h"
#include "MovementComponent.h"
#include "RotationComponent.h"
#include "HpComponent.h"
#include "MpComponent.h"
#include "ApComponent.h"
#include "ActionComponent.h"
#include "BodyViewComponent.h"
#include "MoveToComponent.h"
#include "RotateToComponent.h"
#include "MoveTowardComponent.h"
#include "LockOnComponent.h"

#include <map>
#include <vector>
#include <string>

class GameManager : public Singleton<GameManager>
{

public:

    GameManager();
    ~GameManager();

	unsigned int createGameObject();
	void deleteGameObject(unsigned int id);
	void clearAllGameObjects();

	void setState(unsigned int id, unsigned int state);
	unsigned int getState(unsigned int id);

	void setPosition(unsigned int id, float x, float y, float z);
	void getPosition(unsigned int id, float &x, float &y, float &z);
	void setRotation(unsigned int id, float x, float y, float z);
	void getRotation(unsigned int id, float &x, float &y, float &z);
	void setScale(unsigned int id, float x, float y, float z);
	void getScale(unsigned int id, float &x, float &y, float &z);

	void addGameComponent(unsigned int id /*game object ID*/, ComponentType type);
	void deleteGameComponent(unsigned int id /*game object ID*/, ComponentType type);
	void clearAllGameComponents();
	
	void addResourceID(unsigned int id /*game object ID*/, std::string resourceName, unsigned int resourceID);
	unsigned int getResourceID(unsigned int id /*game object ID*/, std::string resourceName); // return 0 if the resource doen't exist
	void removeResource(unsigned int id /*game object ID*/, std::string resourceName);

	// after this point, we add GameComponent spacific controls
	// we check first if the GameComponent exists, then we change it
	// if we try to control a GameComponent that doesn't exists in a GameObject, we give an error

    // for NameComponent
	void setName(unsigned int id, std::string value);
	std::string getName(unsigned int id);
	
	// for MovementComponent
   	void setSpeedX(unsigned int id, float value);
	float getSpeedX(unsigned int id);
	void setSpeedY(unsigned int id, float value);
	float getSpeedY(unsigned int id);
	void setSpeedZ(unsigned int id, float value);
	float getSpeedZ(unsigned int id);
	void setAccelerationX(unsigned int id, float value);
	float getAccelerationX(unsigned int id);
	void setAccelerationY(unsigned int id, float value);
	float getAccelerationY(unsigned int id);
	void setAccelerationZ(unsigned int id, float value);
	float getAccelerationZ(unsigned int id);

	// for RotationComponent
   	void setRotationSpeedX(unsigned int id, float value);
	float getRotationSpeedX(unsigned int id);
	void setRotationSpeedY(unsigned int id, float value);
	float getRotationSpeedY(unsigned int id);
	void setRotationSpeedZ(unsigned int id, float value);
	float getRotationSpeedZ(unsigned int id);
	void setRotationAccelerationX(unsigned int id, float value);
	float getRotationAccelerationX(unsigned int id);
	void setRotationAccelerationY(unsigned int id, float value);
	float getRotationAccelerationY(unsigned int id);
	void setRotationAccelerationZ(unsigned int id, float value);
	float getRotationAccelerationZ(unsigned int id);

	// for HpComponent
	void setMaxHP(unsigned int id, int value);
	int getMaxHP(unsigned int id);
	void setMinHP(unsigned int id, int value);
	int getMinHP(unsigned int id);
	void setHP(unsigned int id, int value);
	int getHP(unsigned int id);

	// for MpComponent
	void setMaxMP(unsigned int id, int value);
	int getMaxMP(unsigned int id);
	void setMinMP(unsigned int id, int value);
	int getMinMP(unsigned int id);
	void setMP(unsigned int id, int value);
	int getMP(unsigned int id);

	// for ApComponent
	void setMaxAP(unsigned int id, int value);
	int getMaxAP(unsigned int id);
	void setMinAP(unsigned int id, int value);
	int getMinAP(unsigned int id);
	void setAP(unsigned int id, int value);
	int getAP(unsigned int id);

	// for ActionComponent
	void setActionID(unsigned int id, unsigned int value);
	unsigned int getActionID(unsigned int id);
	void setActionPower(unsigned int id, float value);
	float getActionPower(unsigned int id);
	
	// for BodyViewComponent
	void setBodyViewID(unsigned int id, unsigned int value);
	unsigned int getBodyViewID(unsigned int id);
	void setYOffset(unsigned int id, float value);
	float getYOffset(unsigned int id);
	void setRotationOffset(unsigned int id, float value);
	float getRotationOffset(unsigned int id);
	void setRed(unsigned int id, unsigned int value);
	unsigned int getRed(unsigned int id);
	void setGreen(unsigned int id, unsigned int value);
	unsigned int getGreen(unsigned int id);
	void setBlue(unsigned int id, unsigned int value);
	unsigned int getBlue(unsigned int id);
	
	// fot MoveToComponent
	void setMoveToX(unsigned int id, float value);
	float getMoveToX(unsigned int id);
	void setMoveToY(unsigned int id, float value);
	float getMoveToY(unsigned int id);
	void setMoveToZ(unsigned int id, float value);
	float getMoveToZ(unsigned int id);

	// fot RotateToComponent
	void setRotateToX(unsigned int id, float value);
	float getRotateToX(unsigned int id);
	void setRotateToY(unsigned int id, float value);
	float getRotateToY(unsigned int id);
	void setRotateToZ(unsigned int id, float value);
	float getRotateToZ(unsigned int id);

	// fot MoveTowardComponent
	void setMoveTowardX(unsigned int id, float value);
	float getMoveTowardX(unsigned int id);
	void setMoveTowardY(unsigned int id, float value);
	float getMoveTowardY(unsigned int id);
	void setMoveTowardZ(unsigned int id, float value);
	float getMoveTowardZ(unsigned int id);

	// fot LockOnComponent
	void setTarget(unsigned int id, unsigned int value);
	unsigned int getTarget(unsigned int id);
	void addHunter(unsigned int id, unsigned int value);
	void removeHunter(unsigned int id, unsigned int value);
	void clearHunterList(unsigned int id);
	bool doesHunterExists(unsigned int id, unsigned int value);
	int getHunterListSize(unsigned int id);
	unsigned int getHunterByIndex(unsigned int id, int index);

private:

	unsigned int gameIDInc; // what we will use to assigne IDs to GameObject
	unsigned int gCompIDInc; // what we will use to assigne IDs to GameComponent

	std::map<unsigned int, GameObject*> gameObjectList;
	std::map<unsigned int, GameComponent*> gameComponentList;


	bool doGameObjectExists(unsigned int id);
	GameObject *getGameObject(unsigned int id);

	bool doGameComponentExists(unsigned int id);
	GameComponent *getGameComponent(unsigned int id);

	// Component control
	// we use this to get components directly in the manager
	// excetptions are already checked in the getComponent methode
	GameComponent *getComponent(unsigned int id /*game object id*/, ComponentType type);
	NameComponent *getNameComponent(unsigned int id /*game object id*/);	
	MovementComponent *getMovementComponent(unsigned int id /*game object id*/);	
	RotationComponent *getRotationComponent(unsigned int id /*game object id*/);	
	HpComponent *getHPComponent(unsigned int id /*game object id*/);	
	MpComponent *getMPComponent(unsigned int id /*game object id*/);	
	ApComponent *getAPComponent(unsigned int id /*game object id*/);	
	ActionComponent *getActionComponent(unsigned int id /*game object id*/);	
	BodyViewComponent *getBodyViewComponent(unsigned int id /*game object id*/);	
	MoveToComponent *getMoveToComponent(unsigned int id /*game object id*/);	
	RotateToComponent *getRotateToComponent(unsigned int id /*game object id*/);	
	MoveTowardComponent *getMoveTowardComponent(unsigned int id /*game object id*/);	
	LockOnComponent *getLockOnComponent(unsigned int id /*game object id*/);	

};
