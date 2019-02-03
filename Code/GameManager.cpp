#include "GameManager.h"

GameManager *Singleton<GameManager>::sInstance = 0;

GameManager::GameManager()
{
    gameIDInc = 0;
    gCompIDInc = 0;
}

GameManager::~GameManager()
{
    clearAllGameObjects();
    clearAllGameComponents();
}

unsigned int GameManager::createGameObject()
{
    gameIDInc++;
    
    GameObject *go = new GameObject(gameIDInc);
    
    gameObjectList[gameIDInc] = go;
    
    return gameIDInc;
}

void GameManager::deleteGameObject(unsigned int id)
{
    if(doGameObjectExists(id))
    {
		GameObject *go = getGameObject(id);

		// we get the components that this object has
		int listSize = go->componentListSize();
		std::vector<ComponentType> typeList;
		for(int i = 0; i < listSize; i++)
		{
			typeList.push_back(go->getComponentByIndex(i));
		}

		// we remove the components first
		listSize = typeList.size();

		for(int i = 0; i < listSize; i++)
		{
			deleteGameComponent(id, typeList[i]);
		}

		// now we remove the object
        gameObjectList.erase(id);
    }
    else
    {
        // throw something
    }
}

void GameManager::clearAllGameObjects()
{
     gameObjectList.clear();
}

void GameManager::setState(unsigned int id, unsigned int state)
{
    if(doGameObjectExists(id))
        getGameObject(id)->setState(state);
    else
    {
        // throw something
    }
}

unsigned int GameManager::getState(unsigned int id)
{
    if(doGameObjectExists(id))
        return getGameObject(id)->getState();
    else
    {
        // throw something
    }

    return 0;
}

void GameManager::setPosition(unsigned int id, float x, float y, float z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->setPosition(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::getPosition(unsigned int id, float &x, float &y, float &z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->getPosition(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::setRotation(unsigned int id, float x, float y, float z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->setRotation(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::getRotation(unsigned int id, float &x, float &y, float &z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->getRotation(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::setScale(unsigned int id, float x, float y, float z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->setScale(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::getScale(unsigned int id, float &x, float &y, float &z)
{
    if(doGameObjectExists(id))
        getGameObject(id)->getScale(x, y, z);
    else
    {
        // throw something
    }
}

void GameManager::addGameComponent(unsigned int id, ComponentType type)
{
    if(doGameObjectExists(id))
    {
        gCompIDInc++;
        GameComponent *gc = 0;

        switch(type)
        {
        case NAME_COMP:
             
             gc = new NameComponent(gCompIDInc);
             
             break;

        case MOVEMENT_COMP:
             
             gc = new MovementComponent(gCompIDInc);
             
             break;

        case ROTATION_COMP:
             
             gc = new MovementComponent(gCompIDInc);
             
             break;

        case HP_COMP:
             
             gc = new HpComponent(gCompIDInc);
             
             break;

        case MP_COMP:
             
             gc = new MpComponent(gCompIDInc);
             
             break;

        case AP_COMP:
             
             gc = new ApComponent(gCompIDInc);
             
             break;

        case ACTION_COMP:
             
             gc = new ActionComponent(gCompIDInc);
             
             break;

		case BODY_VIEW_COMP:
             
             gc = new BodyViewComponent(gCompIDInc);
             
             break;

        case MOVE_TO_COMP:
             
             gc = new MoveToComponent(gCompIDInc);
             
             break;

        case ROTATE_TO_COMP:
             
             gc = new RotateToComponent(gCompIDInc);
             
             break;

        case MOVE_TOWARD_COMP:
             
             gc = new MoveTowardComponent(gCompIDInc);
             
             break;

		case LOCK_ON_COMP:
             
             gc = new LockOnComponent(gCompIDInc);
             
             break;

		default:
                // throw something
             break;
        }
        
        gameComponentList[gCompIDInc] = gc;
        getGameObject(id)->addComponent(type, gCompIDInc);        
    }
    else
    {
        // throw something
    }
}

void GameManager::deleteGameComponent(unsigned int id, ComponentType type)
{
    if(doGameObjectExists(id))
    {
        unsigned int gcID = getGameObject(id)->getComponent(type);
        
        if(doGameComponentExists(gcID))
        {
            getGameObject(id)->removeComponent(type);
            gameComponentList.erase(gcID);
        }
        else
        {
        }
    }
    else
    {
        // throw
    }
}

void GameManager::clearAllGameComponents()
{
     gameComponentList.clear();
}

void GameManager::addResourceID(unsigned int id, std::string resourceName, unsigned int resourceID)
{
    if(doGameObjectExists(id))
    {
        getGameObject(id)->addResource(resourceName, resourceID);
    }
    else
    {
        // throw something
    }
}

unsigned int GameManager::getResourceID(unsigned int id, std::string resourceName)
{
    if(doGameObjectExists(id))
    {
        return getGameObject(id)->getResource(resourceName);
    }
    else
    {
        // throw something
    }

    return 0;
}

void GameManager::removeResource(unsigned int id, std::string resourceName)
{
    if(doGameObjectExists(id))
    {
        getGameObject(id)->removeResource(resourceName);
    }
    else
    {
        // throw something
    }
}

void GameManager::setName(unsigned int id, std::string value)
{
    getNameComponent(id)->setName(value);
}

std::string GameManager::getName(unsigned int id)
{
    return getNameComponent(id)->getName();
}

void GameManager::setSpeedX(unsigned int id, float value)
{
    getMovementComponent(id)->setSpeedX(value);
}

float GameManager::getSpeedX(unsigned int id)
{
    return getMovementComponent(id)->getSpeedX();
}

void GameManager::setSpeedY(unsigned int id, float value)
{
    getMovementComponent(id)->setSpeedY(value);
}

float GameManager::getSpeedY(unsigned int id)
{
    return getMovementComponent(id)->getSpeedY();
}

void GameManager::setSpeedZ(unsigned int id, float value)
{
    getMovementComponent(id)->setSpeedZ(value);
}

float GameManager::getSpeedZ(unsigned int id)
{
    return getMovementComponent(id)->getSpeedZ();
}

void GameManager::setAccelerationX(unsigned int id, float value)
{
    getMovementComponent(id)->setAccelerationX(value);
}

float GameManager::getAccelerationX(unsigned int id)
{
    return getMovementComponent(id)->getAccelerationX();
}

void GameManager::setAccelerationY(unsigned int id, float value)
{
    getMovementComponent(id)->setAccelerationY(value);
}

float GameManager::getAccelerationY(unsigned int id)
{
    return getMovementComponent(id)->getAccelerationY();
}

void GameManager::setAccelerationZ(unsigned int id, float value)
{
    getMovementComponent(id)->setAccelerationZ(value);
}

float GameManager::getAccelerationZ(unsigned int id)
{
    return getMovementComponent(id)->getAccelerationZ();
}

void GameManager::setRotationSpeedX(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationSpeedX(value);
}

float GameManager::getRotationSpeedX(unsigned int id)
{
    return getRotationComponent(id)->getRotationSpeedX();
}

void GameManager::setRotationSpeedY(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationSpeedY(value);
}

float GameManager::getRotationSpeedY(unsigned int id)
{
    return getRotationComponent(id)->getRotationSpeedY();
}

void GameManager::setRotationSpeedZ(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationSpeedZ(value);
}

float GameManager::getRotationSpeedZ(unsigned int id)
{
    return getRotationComponent(id)->getRotationSpeedZ();
}

void GameManager::setRotationAccelerationX(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationAccelerationX(value);
}

float GameManager::getRotationAccelerationX(unsigned int id)
{
    return getRotationComponent(id)->getRotationAccelerationX();
}

void GameManager::setRotationAccelerationY(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationAccelerationY(value);
}

float GameManager::getRotationAccelerationY(unsigned int id)
{
    return getRotationComponent(id)->getRotationAccelerationY();
}

void GameManager::setRotationAccelerationZ(unsigned int id, float value)
{
    getRotationComponent(id)->setRotationAccelerationZ(value);
}

float GameManager::getRotationAccelerationZ(unsigned int id)
{
    return getRotationComponent(id)->getRotationAccelerationZ();
}

void GameManager::setMaxHP(unsigned int id, int value)
{
    getHPComponent(id)->setMaxHP(value);
}

int GameManager::getMaxHP(unsigned int id)
{
    return getHPComponent(id)->getMaxHP();
}

void GameManager::setMinHP(unsigned int id, int value)
{
    getHPComponent(id)->setMinHP(value);
}

int GameManager::getMinHP(unsigned int id)
{
    return getHPComponent(id)->getMinHP();
}
void GameManager::setHP(unsigned int id, int value)
{
    getHPComponent(id)->setHP(value);
}

int GameManager::getHP(unsigned int id)
{
    return getHPComponent(id)->getHP();
}

void GameManager::setMaxMP(unsigned int id, int value)
{
    getMPComponent(id)->setMaxMP(value);
}

int GameManager::getMaxMP(unsigned int id)
{
    return getMPComponent(id)->getMaxMP();
}

void GameManager::setMinMP(unsigned int id, int value)
{
    getMPComponent(id)->setMinMP(value);
}

int GameManager::getMinMP(unsigned int id)
{
    return getMPComponent(id)->getMinMP();
}

void GameManager::setMP(unsigned int id, int value)
{
    getMPComponent(id)->setMP(value);
}

int GameManager::getMP(unsigned int id)
{
    return getMPComponent(id)->getMP();
}

void GameManager::setMaxAP(unsigned int id, int value)
{
    getAPComponent(id)->setMaxAP(value);
}

int GameManager::getMaxAP(unsigned int id)
{
    return getAPComponent(id)->getMaxAP();
}

void GameManager::setMinAP(unsigned int id, int value)
{
    getAPComponent(id)->setMinAP(value);
}

int GameManager::getMinAP(unsigned int id)
{
    return getAPComponent(id)->getMinAP();
}

void GameManager::setAP(unsigned int id, int value)
{
    getAPComponent(id)->setAP(value);
}

int GameManager::getAP(unsigned int id)
{
    return getAPComponent(id)->getAP();
}

void GameManager::setActionID(unsigned int id, unsigned int value)
{
	getActionComponent(id)->setActionID(value);
}

unsigned int GameManager::getActionID(unsigned int id)
{
	return getActionComponent(id)->getActionID();
}

void GameManager::setActionPower(unsigned int id, float value)
{
	getActionComponent(id)->setActionPower(value);
}

float GameManager::getActionPower(unsigned int id)
{
	return getActionComponent(id)->getActionPower();
}

void GameManager::setBodyViewID(unsigned int id, unsigned int value)
{
    getBodyViewComponent(id)->setBodyViewID(value);
}

unsigned int GameManager::getBodyViewID(unsigned int id)
{
    return getBodyViewComponent(id)->getBodyViewID();
}

void GameManager::setYOffset(unsigned int id, float value)
{
	getBodyViewComponent(id)->setYOffset(value);
}

float GameManager::getYOffset(unsigned int id)
{
	return getBodyViewComponent(id)->getYOffset();
}

void GameManager::setRotationOffset(unsigned int id, float value)
{
	getBodyViewComponent(id)->setRotationOffset(value);
}

float GameManager::getRotationOffset(unsigned int id)
{
	return getBodyViewComponent(id)->getRotationOffset();
}

void GameManager::setRed(unsigned int id, unsigned int value)
{
    getBodyViewComponent(id)->setRed(value);
}

unsigned int GameManager::getRed(unsigned int id)
{
    return getBodyViewComponent(id)->getRed();
}

void GameManager::setGreen(unsigned int id, unsigned int value)
{
    getBodyViewComponent(id)->setGreen(value);
}

unsigned int GameManager::getGreen(unsigned int id)
{
    return getBodyViewComponent(id)->getGreen();
}

void GameManager::setBlue(unsigned int id, unsigned int value)
{
    getBodyViewComponent(id)->setBlue(value);
}

unsigned int GameManager::getBlue(unsigned int id)
{
    return getBodyViewComponent(id)->getBlue();
}

void GameManager::setMoveToX(unsigned int id, float value)
{
    getMoveToComponent(id)->setMoveToX(value);
}

float GameManager::getMoveToX(unsigned int id)
{
    return getMoveToComponent(id)->getMoveToX();
}

void GameManager::setMoveToY(unsigned int id, float value)
{
    getMoveToComponent(id)->setMoveToY(value);
}

float GameManager::getMoveToY(unsigned int id)
{
    return getMoveToComponent(id)->getMoveToY();
}

void GameManager::setMoveToZ(unsigned int id, float value)
{
    getMoveToComponent(id)->setMoveToZ(value);
}

float GameManager::getMoveToZ(unsigned int id)
{
    return getMoveToComponent(id)->getMoveToZ();
}

void GameManager::setRotateToX(unsigned int id, float value)
{
    getRotateToComponent(id)->setRotateToX(value);
}

float GameManager::getRotateToX(unsigned int id)
{
    return getRotateToComponent(id)->getRotateToX();
}

void GameManager::setRotateToY(unsigned int id, float value)
{
    getRotateToComponent(id)->setRotateToY(value);
}

float GameManager::getRotateToY(unsigned int id)
{
    return getRotateToComponent(id)->getRotateToY();
}

void GameManager::setRotateToZ(unsigned int id, float value)
{
    getRotateToComponent(id)->setRotateToZ(value);
}

float GameManager::getRotateToZ(unsigned int id)
{
    return getRotateToComponent(id)->getRotateToZ();
}

void GameManager::setMoveTowardX(unsigned int id, float value)
{
    getMoveTowardComponent(id)->setMoveTowardX(value);
}

float GameManager::getMoveTowardX(unsigned int id)
{
    return getMoveTowardComponent(id)->getMoveTowardX();
}

void GameManager::setMoveTowardY(unsigned int id, float value)
{
    getMoveTowardComponent(id)->setMoveTowardY(value);
}

float GameManager::getMoveTowardY(unsigned int id)
{
    return getMoveTowardComponent(id)->getMoveTowardY();
}

void GameManager::setMoveTowardZ(unsigned int id, float value)
{
    getMoveTowardComponent(id)->setMoveTowardZ(value);
}

float GameManager::getMoveTowardZ(unsigned int id)
{
    return getMoveTowardComponent(id)->getMoveTowardZ();
}

void GameManager::setTarget(unsigned int id, unsigned int value)
{
    getLockOnComponent(id)->setTarget(value);
}

unsigned int GameManager::getTarget(unsigned int id)
{
    return getLockOnComponent(id)->getTarget();
}

void GameManager::addHunter(unsigned int id, unsigned int value)
{
	getLockOnComponent(id)->addHunter(value);
}

void GameManager::removeHunter(unsigned int id, unsigned int value)
{
	getLockOnComponent(id)->removeHunter(value);
}

void GameManager::clearHunterList(unsigned int id)
{
	getLockOnComponent(id)->clearHunterList();
}

bool GameManager::doesHunterExists(unsigned int id, unsigned int value)
{
	return getLockOnComponent(id)->doesHunterExists(value);
}

int GameManager::getHunterListSize(unsigned int id)
{
	return getLockOnComponent(id)->getHunterListSize();
}

unsigned int GameManager::getHunterByIndex(unsigned int id, int index)
{
	return getLockOnComponent(id)->getHunterByIndex(index);
}

bool GameManager::doGameObjectExists(unsigned int id)
{
    return (gameObjectList.find(id) != gameObjectList.end());
}

GameObject *GameManager::getGameObject(unsigned int id)
{
    return gameObjectList[id];
}

bool GameManager::doGameComponentExists(unsigned int id)
{
    return (gameComponentList.find(id) != gameComponentList.end());
}

GameComponent *GameManager::getGameComponent(unsigned int id)
{
    return gameComponentList[id];
}

// Component control
GameComponent *GameManager::getComponent(unsigned int id, ComponentType type)
{
	// check if GameObject exists
	if(doGameObjectExists(id))
	{
		GameObject *gb = getGameObject(id);

		// does it have the spacific Component type
		unsigned int cID = gb->getComponent(type);

		if(doGameComponentExists(cID))
		{
			// we get the component
			return getGameComponent(cID);
		}
		else // we don't have the component
		{
			//throw some error
		}

	}
	else // GameObjectID doesn't exists
	{
		// throw some error
	}

    return 0;
}

NameComponent *GameManager::getNameComponent(unsigned int id)
{
    return (NameComponent*)getComponent(id, NAME_COMP);
}

MovementComponent *GameManager::getMovementComponent(unsigned int id)
{
    return (MovementComponent*)getComponent(id, MOVEMENT_COMP);
}

RotationComponent *GameManager::getRotationComponent(unsigned int id)
{
    return (RotationComponent*)getComponent(id, ROTATION_COMP);
}

HpComponent *GameManager::getHPComponent(unsigned int id)
{
    return (HpComponent*)getComponent(id, HP_COMP);
}

MpComponent *GameManager::getMPComponent(unsigned int id)
{
    return (MpComponent*)getComponent(id, MP_COMP);
}

ApComponent *GameManager::getAPComponent(unsigned int id)
{
	return (ApComponent*)getComponent(id, AP_COMP);
}

ActionComponent *GameManager::getActionComponent(unsigned int id)
{
	return (ActionComponent*)getComponent(id, ACTION_COMP);
}

BodyViewComponent *GameManager::getBodyViewComponent(unsigned int id)
{
    return (BodyViewComponent*)getComponent(id, BODY_VIEW_COMP);
}

MoveToComponent *GameManager::getMoveToComponent(unsigned int id)
{
    return (MoveToComponent*)getComponent(id, MOVE_TO_COMP);
}

RotateToComponent *GameManager::getRotateToComponent(unsigned int id)
{
    return (RotateToComponent*)getComponent(id, ROTATE_TO_COMP);
}

MoveTowardComponent *GameManager::getMoveTowardComponent(unsigned int id)
{
    return (MoveTowardComponent*)getComponent(id, MOVE_TOWARD_COMP);
}

LockOnComponent *GameManager::getLockOnComponent(unsigned int id)
{
    return (LockOnComponent*)getComponent(id, LOCK_ON_COMP);
}
