#pragma once

#include "StateBase.h"

#include "Game.h"
#include "GraphicManager.h"
#include "InputManager.h"
#include "DataBaseManager.h"
#include "SoundManager.h"

class SplashState : public CStateBase
{

public:

	SplashState();
	~SplashState();

	virtual void Init(void);
	virtual void DeInit(void);

	virtual void Pause(CStateEngine *engine);
	virtual void Resume(CStateEngine *engine);

	virtual void HandleEvents(CStateEngine *engine);
	virtual void Update(CStateEngine *engine);
	virtual void Draw(CStateEngine *engine);

private:

	Game *game;
	GraphicManager *gMgr;
	InputManager *iMgr;
	DataBaseManager *dbMgr;
	SoundManager *sMgr;

	unsigned int imageID;
	unsigned int imageID2;

	float wait1; // how long before we start to fade in
	float fadeInTimer; // how long is the fade in
	float wait2; // how long is image shown
	float fadeOutTimer; // how long is the fade out
	float wait3; // how long before we move to the MainMenuState

	float timer; // uses to advance the other timers
	unsigned int state; //0 == wait1, 1 == fade in, 2 == wait2, 3 == fade out, 4 == wait3

	bool goToMainMenu;

};