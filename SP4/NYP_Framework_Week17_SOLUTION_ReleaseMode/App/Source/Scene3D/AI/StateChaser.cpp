#include "StateChaser.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float COOLDOWN_DROP_RATE = 1.f;

StateIdle::StateIdle(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateIdle::~StateIdle()
{
}

void StateIdle::Enter()
{
}

void StateIdle::Update(double dt)
{
	//if (m_go->nearest == nullptr || m_go->nearest->active == false || m_go->nearest->sm->GetCurrentState() == "Dead")
	{
		//if (m_go->type == GameObject::GO_CHASER)
		{
			MessageWRU* msg = new MessageWRU(m_go, MessageWRU::Player, m_go->range * SceneData::GetInstance()->GetGridSize());
			PostOffice::GetInstance()->Send("Scene", msg);
		}
	}

	if (m_go->nearest != nullptr)
	{
		if (m_go->nearest->type == GameObject::GO_PLAYER)
		{
			if ((m_go->pos - m_go->nearest->pos).Length() <= m_go->range + 1)
				m_go->sm->SetNextState("Chase");
		}
	}

	if (m_go->move == true)
	{
		m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = true;
		if (m_go->nearest != nullptr && m_go->nearest->type)
		{
			float dx = m_go->nearest->pos.x - m_go->pos.x;
			float dy = m_go->nearest->pos.y - m_go->pos.y;
			if (dx > 0)
				m_go->moveLeft = false;
			else
				m_go->moveRight = false;
			if (dy > 0)
				m_go->moveDown = false;
			else
				m_go->moveUp = false;
			if (Math::FAbs(dx) > Math::FAbs(dy))
				m_go->moveDown = m_go->moveUp = false;
			else
				m_go->moveLeft = m_go->moveRight = false;
		}
	}
	else if (m_go->move == false)
		m_go->moveLeft = m_go->moveRight = m_go->moveUp = m_go->moveDown = false;
}

void StateIdle::Exit()
{
}

StateChase::StateChase(const std::string& stateID, GameObject* go)
	: State(stateID),
	m_go(go)
{
}

StateChase::~StateChase()
{
}

void StateChase::Enter()
{

}

void StateChase::Update(double dt)
{
	//if (m_go->nearest == nullptr || m_go->nearest->active == false || m_go->nearest->sm->GetCurrentState() == "Dead")
	{
		//if (m_go->type == GameObject::GO_CHASER)
		{
			MessageWRU* msg = new MessageWRU(m_go, MessageWRU::Player, m_go->range * SceneData::GetInstance()->GetGridSize());
			PostOffice::GetInstance()->Send("Scene", msg);
		}
	}

	if (m_go->nearest != nullptr)
	{
		if (m_go->nearest->type == GameObject::GO_PLAYER)
		{
			if ((m_go->pos - m_go->nearest->pos).Length() > m_go->range)
				m_go->sm->SetNextState("Idle");
		}
	}
}

void StateChase::Exit()
{
}
