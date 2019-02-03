#include "StateEngine.h"

void CStateEngine::Init()
{
	m_isRunning = true;
	requestState = 0;
	requestID = 0;
}

void CStateEngine::DeInit()
{
	// delete all states
	while(!m_states.empty())
	{
		m_states.back()->DeInit();
		m_states.pop_back();
	}
}

void CStateEngine::ChangeState(CStateBase *state)
{
	// delete all states
	while(!m_states.empty())
	{
		m_states.back()->DeInit();
		//delete m_states.back();
		m_states.pop_back();
	}

	// store in the new state and do Init()
	m_states.push_back(state);
	m_states.back()->Init();
}

void CStateEngine::PushState(CStateBase *state)
{
	// make sure if the states vector is not empty then pause the current state
	if(!m_states.empty())
		m_states.back()->Pause(this);

	// now store the new state and do Init()
	m_states.push_back(state);
	m_states.back()->Init();
}

void CStateEngine::PopState()
{
	// delete the current state
	if(!m_states.empty())
	{
		m_states.back()->DeInit();
		m_states.pop_back();
	}

	// now resume the previous state
	if(!m_states.empty())
		m_states.back()->Resume(this);
}

void CStateEngine::RequestChangeState(CStateBase *state)
{
	requestState = state;
	requestID = 1;
}

void CStateEngine::RequestPushState(CStateBase *state)
{
	requestState = state;
	requestID = 2;
}

void CStateEngine::RequestPopState(void)
{
	requestID = 3;
}

void CStateEngine::CheckRequests(void)
{
	switch(requestID)
	{

	case 0:
		break;

	case 1:

		ChangeState(requestState);

		break;

	case 2:

		PushState(requestState);

		break;

	case 3:

		PopState();

		break;

	}

	requestID = 0;
	requestState = 0;
}

void CStateEngine::HandleEvents()
{
	m_states.back()->HandleEvents(this);
}

void CStateEngine::Update()
{
	m_states.back()->Update(this);
}

void CStateEngine::Draw()
{
	m_states.back()->Draw(this);
}

bool CStateEngine::IsRunning()
{
	return m_isRunning;
}

void CStateEngine::Quit()
{
	m_isRunning = false;
}
