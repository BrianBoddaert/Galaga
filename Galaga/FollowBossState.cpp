#include "stdafx.h"
#include "FollowBossState.h"
#include "BombRunState.h"
#include <SDL.h>
#include "EnemyManager.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "AIFlyComponent.h"

using namespace Willem;

FollowBossState::FollowBossState(Willem::GameObject* go, std::weak_ptr<Willem::GameObject> boss) noexcept
	:AlienState{ go }
	, m_pBoss{boss}
	, m_BossAlive{true}
{
	Enter();
}

FollowBossState::~FollowBossState()
{
	Exit();
}


void FollowBossState::Update(float)
{
	if (!m_BossAlive)
		return;

	if (m_pBoss.use_count() <= 1)
		m_BossAlive = false;
}

AlienState* FollowBossState::GetFollowUpState() const
{
	return nullptr;
}

void FollowBossState::Enter()
{


}

void FollowBossState::Exit()
{
	if (m_pBoss.use_count() <= 1)
		m_BossAlive = false;
	else
	{
		m_pBoss.lock()->GetComponent<AIFlyComponent>()->DisableCapturedPlayerActive();
	}
	
}