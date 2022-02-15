#ifndef STATE_H
#define STATE_H

#include <string>

class State
{
	const std::string m_stateID;
protected:
	State(const std::string &stateID);
public:
	virtual ~State();
	const std::string& GetStateID();

	//To be implemented by concrete states
	virtual void Enter() = 0;
	virtual void Update(double dt) = 0;
	virtual void Exit() = 0;
};

#endif