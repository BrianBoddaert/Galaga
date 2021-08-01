#include "stdafx.h"
#include "SpawnDiveState.h"
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

SpawnDiveState::SpawnDiveState(Willem::GameObject* go, Dive* dive) noexcept
	:AlienState{go}
	, m_pDive{ dive }
{
	Enter();
}

SpawnDiveState::~SpawnDiveState()
{
	Exit();
}

void SpawnDiveState::Update(float deltaT)
{
	m_pDive->Update(deltaT);
	
	if (m_pDive->GetCompleted())
		m_StateFinished = true;

}
void SpawnDiveState::Enter()
{
}
void SpawnDiveState::Exit() 
{
	if (m_pDive)
		delete m_pDive;
}

AlienState* SpawnDiveState::GetFollowUpState() const
{
	return new FormationState(m_pGameObject);
}