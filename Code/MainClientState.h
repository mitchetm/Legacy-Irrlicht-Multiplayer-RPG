#pragma once

#include "StateBase.h"

#include "Game.h"
#include "GameManager.h"
#include "GraphicManager.h"
#include "InputManager.h"
#include "DataBaseManager.h"
#include "NetWorkManager.h"
#include "SoundManager.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class MainClientState : public CStateBase
{

public:

	MainClientState();
	~MainClientState();

	virtual void Init(void);
	virtual void DeInit(void);

	virtual void Pause(CStateEngine *engine);
	virtual void Resume(CStateEngine *engine);

	virtual void HandleEvents(CStateEngine *engine);
	virtual void Update(CStateEngine *engine);
	virtual void Draw(CStateEngine *engine);

	void creatChatBox(void);

	bool moveChatToHistory(void);

	void addNewUser(unsigned int netID, float x, float y, float z, float yAngle, char *name, unsigned int modelNumber);
	void removeUser(unsigned int netID);

	void updateCameraLook(void);
	void updatePlayerMovement(float deltaTime);
	void updateCameraPosition(void);
	void updatePlayerDirection(void);
	void updateAnimationChange(void);

	void checkAndSendUpdate(float deltaTime);

	void updateInterface(void);
	void updateNetObjects(float deltaTime);

	unsigned int findNearestObject(void);
	void lockOnTarget(unsigned int objectID);
	void sendLockOnTarget(unsigned int objectID);

	void updateHP(void);

	void attack(void);
	void sendAttack(void); // we tell the server we are attacking

	void block(void);
	void stopBlock(void);
	void sendBlock(void);
	void sendStopBlock(void);

	bool checkForDeath(void);
	void sendDeath(void);

	void respawn(void);
	void sendRespawn(void);

	// remove later
	void updateGolemArea(float deltaTime);

	void shootFireBall(void);
	unsigned int prepareFireBall(void);
	void setFireBallSpeed(unsigned int id);
	void updateFireBalls(float deltaTime);

private:

	// database helpers
	void getModelAndTextureFromDB(unsigned int id, std::string &modelPath, std::vector<std::string> &texturePathList, float &yOffset, float &rotationOffset, float &scaleOffset);
	void getAnimationInfoFromDB(unsigned int id, char *animeName, int &start, int &end, float &speed, bool &loop);

	GameManager *gameMgr;
	Game *game;
	GraphicManager *gMgr;
	InputManager *iMgr;
	DataBaseManager *dbMgr;
	NetworkManager *netMgr;
	SoundManager *sMgr;

	std::string userName;

	// for the chat box
	unsigned int chatSubmitButtonID;
	unsigned int chatEditBoxID;
	unsigned int chatHistoryEditBoxID;
	unsigned int WeirdMap;

	// the game object id for the main character and the camera
	unsigned int myID;
	unsigned int cameraID;

	int moveForward, moveSide;
	float speed;

	unsigned int mouseStaticID; // to see the value of the mouse

	float maxXAngle; // limit for looking up
	float minXAngle; // limit for looking down

	float cameraFarOut; // how far is the camera away from the player

	unsigned int golem1ID;
	unsigned int golem2ID;

	unsigned int mainThemeChannleID;

	float arenaZLimit; // how far before we are offically in side the Golem battel arena
	bool inArena; // are we in the Golem battel arena?
	float soundFadeSpeed;
	float soundVolume;
	int fadeState; // 0 = no fade, 1 = fade out main song, 2 = fade in battel song, 3 = fade out battel song, 4 = fade in theme song

	// network stuff
	std::map<unsigned int, unsigned int> netObjectList;

	bool isNetActive;
	float netUpdateTimer;

	bool hasMoved; // we need these so we don't have to send updates unless they have moved or turned
	bool oldHasMoved; // were we moving last time
	bool hasTurned;
	bool justStopped; // this is to check if the character stopped moving. We will need to send one more update when we stop

	bool isRightClickDown;
	bool isLockOnDown; // to check if we press the lock down button, for now it will be 1
	bool isAttackDown; // to check if attack button is down

	float spawnTimer;

	// temp
	bool isHPupDown;
	bool isHPlowDown;

	unsigned int Meta;
	unsigned int TS;

	std::vector<unsigned int> fireBallList;
	unsigned int maxFireBalls;
	unsigned int currentNumberOfFireBalls;
	float fireBallSpeed;
	float fireBallCoolDown;
	float fireBallReady;
	float fireBallLifeSpan;
	bool isFireDown;

	unsigned int Inventory;
	unsigned int Slot;
	bool isInventory;
	bool isInvEvents;

	bool isJump;

	unsigned int testObjectID;

	// we add a way to check if chat box is active
	// if we press Enter or clicked on the chat box, we activat it
	// no other keys are accepted as long as the chat box is active
	// if press Enter while the chat box is empty, we unactivate it
	bool isChatBoxActive;
	bool enterKeyDown;

	bool lockOnChange; // we use to check if the player locked on to a target, changed locked on, or lock off. We need to update whne that happesn.

	unsigned int exitMessageBoxID;
	bool holdingESC;

	float apRunLimitValue;
	float apRunLimit;
	float apDecreaseSpeedValue;
	float apDecreaseSpeed;

	float apBlockLimitValue;
	float apBlockLimit;
	float apIncreaseSpeedValue;
	float apIncreaseSpeed;

	bool frezz;
	float frezzIncreaseSpeedValue;
	float frezzIncreaseSpeed;
};
