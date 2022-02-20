#include "State.h"

State::State(const std::string & stateID)
	: m_stateID(stateID)
{
}

State::~State()
{
}

const std::string & State::GetStateID()
{
	return m_stateID;
}
