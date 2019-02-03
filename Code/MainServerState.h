#pragma once

#include "StateBase.h"

#include "GameManager.h"
#include "Game.h"
#include "NetworkManager.h"

#include <map>
#include <vector>

// messages classes that are sent later in the update function

class updateMsg
{

public:

	updateMsg(unsigned int id, float xx, float yy, float zz, float yyAngle, RakNet::Time ts)
	{
		netID = id;
		x = xx;
		y = yy;
		z = zz;
		yAngle = yyAngle;
		timeStamp = ts;
	}

	unsigned int netID;
	float x;
	float y;
	float z;
	float yAngle;
	RakNet::Time timeStamp;
};

class MainServerState : public CStateBase
{

public:

	MainServerState();
	~MainServerState();

	virtual void Init(void);
	virtual void DeInit(void);

	virtual void Pause(CStateEngine *engine);
	virtual void Resume(CStateEngine *engine);

	virtual void HandleEvents(CStateEngine *engine);
	virtual void Update(CStateEngine *engine);
	virtual void Draw(CStateEngine *engine);

	void createCharacter(unsigned int netID, char* userName, unsigned int modelNumber);

	void removeUser(unsigned int userID);

private:

	GameManager *gameMgr;
	Game *game;
	NetworkManager *netMgr;

	std::map<unsigned int, unsigned int> netObjectList;

	std::vector<unsigned int> newUsers;
	std::vector<unsigned int> waitListUsers;
	std::vector<unsigned int> activeUsers;

	std::map<unsigned int, std::vector<unsigned int>> confirmList;
};
