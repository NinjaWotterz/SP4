#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <string>
#include <map>
#include "State.h"

class StateMachine
{
	std::map<std::string, State*> m_stateMap;
	State *m_currState;
	State *m_nextState;
public:
	StateMachine();
	~StateMachine();
	void AddState(State *newState);
	void SetNextState(const std::string &nextStateID);
	const std::string& GetCurrentState();
	void Update(double dt);
};

#endif