#include "StateMachine.h"

StateMachine::StateMachine()
	: m_currState(NULL),
	m_nextState(NULL)
{
}

StateMachine::~StateMachine()
{
	for (std::map<std::string, State*>::iterator it = m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		delete it->second;
	}
	m_stateMap.clear();
}

void StateMachine::AddState(State *newState)
{
	if (!newState)
		return;
	if (m_stateMap.find(newState->GetStateID()) != m_stateMap.end())
		return;
	if (!m_currState)
		m_currState = m_nextState = newState;
	m_stateMap.insert(std::pair<std::string, State*>(newState->GetStateID(), newState));
}

void StateMachine::SetNextState(const std::string &nextStateID)
{
	std::map<std::string, State*>::iterator it = m_stateMap.find(nextStateID);
	if (it != m_stateMap.end())
	{
		m_nextState = (State *)it->second;
	}
}

const std::string & StateMachine::GetCurrentState()
{
	if (m_currState)
		return m_currState->GetStateID();
	return "<No states>";
}

void StateMachine::Update(double dt)
{
	if (m_nextState != m_currState)
	{
		m_currState->Exit();
		m_currState = m_nextState;
		m_currState->Enter();
	}
	m_currState->Update(dt);
}
