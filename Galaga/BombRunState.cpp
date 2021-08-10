#include "stdafx.h"
#include "BombRunState.h"
#include "GameObject.h"
#include <iostream>
#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"
#include <cmath>
#include "FormationState.h"
#include "Dive.h"
#include "Dive_UpToRight.h"
#include "Dive_UpToLeft.h"

using namespace Willem;

BombRunState::BombRunState(Willem::GameObject* go, Dive* dive) noexcept
	:AlienState{ go }
	, m_pDive{ dive }
{
	Enter();
}

BombRunState::~BombRunState()
{
	Exit();
}

void BombRunState::Update(float deltaT)
{
	m_pDive->Update(deltaT);

	if (m_pDive->GetCompleted())
		m_StateFinished = true;

}
void BombRunState::Enter()
{
	EnemyManager::GetInstance().AdjustBombRunCounter(1);
}
void BombRunState::Exit()
{
	EnemyManager::GetInstance().AdjustBombRunCounter(-1);
	if (m_pDive)
		delete m_pDive;
}

AlienState* BombRunState::GetFollowUpState() const
{
	return new FormationState(m_pGameObject);
}