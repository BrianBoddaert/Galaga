#include "stdafx.h"
#include "AIFlyComponent.h"
#include "TransformComponent.h"
#include "Minigin.h"
#include "RenderComponent.h"
#include "SpawnDiveState.h"
#include "EnemyManager.h"
#include <cmath>

using namespace Willem;

AIFlyComponent::AIFlyComponent(Willem::GameObject* gameObject)
	:m_pState{new SpawnDiveState(gameObject)}
{
}

AIFlyComponent::~AIFlyComponent()
{
	EnemyManager::GetInstance().UnclaimSpotInFormation(m_pGameObject);
	delete m_pState;
}
void AIFlyComponent::Update(float deltaT)
{
	m_pState->Update(deltaT);

	if (m_pState->GetStateFinished())
	{
		AlienState* followUpState = m_pState->GetFollowUpState();
		delete m_pState;
		m_pState = followUpState;
	}

}

