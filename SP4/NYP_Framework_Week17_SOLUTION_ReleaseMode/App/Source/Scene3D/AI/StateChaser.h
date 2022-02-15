#ifndef STATES_CHASER_H
#define STATES_CHASER_H

#include "State.h"
#include "GameObject.h"

class StateIdle2 : public State
{
	GameObject* m_go;
public:
	StateIdle2(const std::string& stateID, GameObject* go);
	virtual ~StateIdle2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

class StateChase2 : public State
{
	GameObject* m_go;
public:
	StateChase2(const std::string& stateID, GameObject* go);
	virtual ~StateChase2();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif