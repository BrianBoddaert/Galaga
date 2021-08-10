#include "stdafx.h"
#include "FollowBossState.h"
#include "BombRunState.h"
#include <SDL.h>
#include "EnemyManager.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "AIFlyComponent.h"
#include "DSS_MoveToPoint.h"

using namespace Willem;

FollowBossState::FollowBossState(Willem::GameObject* go, std::weak_ptr<Willem::GameObject> boss) noexcept
	:AlienState{ go }
	, m_pBoss{boss}
	, m_BossAlive{true}
	, m_pSubState{nullptr}
{
	Enter();
}

FollowBossState::~FollowBossState()
{
	Exit();
}


void FollowBossState::Update(float deltaT)
{
	if (!m_BossAlive)
		return;

	if (m_pBoss.use_count() <= 1)
	{
		m_BossAlive = false;
		return;
	}

	const float offset = -40.0f;
	const float minimalOffsetToMove = 5.0f;
	Vector2 pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	Vector2 bossPos = m_pBoss.lock()->GetComponent<TransformComponent>()->GetPosition();
	Vector2 bossPosWithOffset = bossPos + Vector2(0, offset);

	if ((bossPosWithOffset - pos).Magnitude() >= minimalOffsetToMove)
	{
		m_pSubState->SetDestinationAndCalculateDirection(bossPosWithOffset);
		m_pSubState->Update(deltaT);
	}

}

AlienState* FollowBossState::GetFollowUpState() const
{
	return nullptr;
}

void FollowBossState::Enter()
{
	if (m_pBoss.use_count() <= 1)
	{
		m_BossAlive = false;
		return;
	}

	Vector2 pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	Vector2 dir = (m_pBoss.lock()->GetComponent<TransformComponent>()->GetPosition() - pos).Normalize();
	m_pSubState = new DSS_MoveToPoint(m_pGameObject, pos, dir);
}

void FollowBossState::Exit()
{
	if (m_pBoss.use_count() <= 1)
		m_BossAlive = false;
	else
	{
		m_pBoss.lock()->GetComponent<AIFlyComponent>()->DisableCapturedPlayerActive();
	}
	

	if (m_pSubState)
		delete m_pSubState;
}