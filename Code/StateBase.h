#pragma once

#include "StateEngine.h"

class CStateEngine;

class CStateBase
{

public:

	virtual void Init(void) = 0;
	virtual void DeInit(void) = 0;

	virtual void Pause(CStateEngine *engine) = 0;
	virtual void Resume(CStateEngine *engine) = 0;

	virtual void HandleEvents(CStateEngine *engine) = 0;
	virtual void Update(CStateEngine *engine) = 0;
	virtual void Draw(CStateEngine *engine) = 0;

};
