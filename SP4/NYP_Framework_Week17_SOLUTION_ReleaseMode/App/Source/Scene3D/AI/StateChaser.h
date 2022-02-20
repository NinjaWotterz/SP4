#ifndef STATES_CHASER_H
#define STATES_CHASER_H

#include "State.h"
#include "GameObject.h"

class StateIdle : public State
{
	GameObject* m_go;
public:
	StateIdle(const std::string& stateID, GameObject* go);
	virtual ~StateIdle();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateChase : public State
{
	GameObject* m_go;
public:
	StateChase(const std::string& stateID, GameObject* go);
	virtual ~StateChase();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif
