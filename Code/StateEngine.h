#pragma once

#include "StateBase.h"
#include <vector>

using namespace std;

class CStateBase;

class CStateEngine
{

public:

	void Init(void);
	void DeInit(void);

	void ChangeState(CStateBase *state);
	void PushState(CStateBase *state);
	void PopState(void);

	void RequestChangeState(CStateBase *state);
	void RequestPushState(CStateBase *state);
	void RequestPopState(void);

	void CheckRequests(void);

	void HandleEvents(void);
	void Update(void);
	void Draw(void);

	bool IsRunning(void);
	void Quit(void);

private:

	vector<CStateBase*> m_states;
	CStateBase* requestState;
	unsigned int requestID; // what type of request? 1 = change; 2 = push, 3 = pop
	bool m_isRunning;

};
