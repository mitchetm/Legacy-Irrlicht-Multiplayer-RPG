#pragma once

#include "StateBase.h"

#include "Game.h"
#include "GraphicManager.h"
#include "InputManager.h"
#include "DataBaseManager.h"
#include "NetworkManager.h"

#include <map>
#include <string>

class MainMenuState : public CStateBase
{

public:

	MainMenuState();
	~MainMenuState();

	virtual void Init(void);
	virtual void DeInit(void);

	virtual void Pause(CStateEngine *engine);
	virtual void Resume(CStateEngine *engine);

	virtual void HandleEvents(CStateEngine *engine);
	virtual void Update(CStateEngine *engine);
	virtual void Draw(CStateEngine *engine);

	bool checkButtonsWereClicked(void);
	void checkEnterWasHitInAnEditBox(void);
	//void checkColorSelectChange(void);
	//void checkIfColorChagne(void);
	void checkSpinBoxChange(void);

	void addUserNameAndStartConnection(void);
	bool handleNetworkMessages(void); // return true if we get a connection and want to go to the client state

	void getCharacterList(void);

private:

	Game *game;
	GraphicManager *gMgr;
	InputManager *iMgr;
	DataBaseManager *dbMgr;
	NetworkManager *netMgr;

	// temp stuff, remove later
	float x, z;
	float speedX, speedZ;
	unsigned int testObjectID;
	float cameraYoffset, cameraZoffset;

	//unsigned int startButtonID;
	//unsigned int optionsButtonID;
	//unsigned int quitButtonID;

	unsigned int userNameID;
	unsigned int passwordID;
	//unsigned int ipID;
	//unsigned int portID;
	unsigned int characterSelectSpinBoxID;
	unsigned int characterSelectStringID;
	unsigned int startButtonID;
	unsigned int optionsButtonID;
	unsigned int quitButtonID;

	//unsigned int redColorID;
	//unsigned int greenColorID;
	//unsigned int blueColorID;
	//unsigned int colorID;

	unsigned int messageBoxID;

	bool holdingESC; // this is so we don't catch an ESC event be accident

	std::map<unsigned int, std::string> selectionList;
};