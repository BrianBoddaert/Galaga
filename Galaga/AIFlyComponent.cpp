#include "stdafx.h"
#include "AIFlyComponent.h"
#include "TransformComponent.h"
#include "Minigin.h"
#include "RenderComponent.h"
#include "SpawnDiveState.h"
#include "EnemyManager.h"
#include <cmath>

using namespace Willem;

AIFlyComponent::AIFlyComponent(Willem::GameObject*)
	:m_pState{ nullptr }
	, m_Speed{ 200.0f }
	, m_RotationSpeed{ 4.0f }
	, m_RotationRadians{ float(M_PI) }
{
}

AIFlyComponent::AIFlyComponent(Willem::GameObject*, SpawnDiveState* state)
	:m_pState{ state }
	, m_Speed{ 200.0f }
	, m_RotationSpeed{4.0f}
	, m_RotationRadians{float(M_PI)}
{
}

void AIFlyComponent::SetSpawnDiveState(SpawnDiveState* state)
{
	m_pState = state;
}

AIFlyComponent::~AIFlyComponent()
{
	EnemyManager::GetInstance().UnclaimSpotInFormation(m_pGameObject);
	delete m_pState;
}
void AIFlyComponent::Update(float deltaT)
{
	if (!m_pState)
		return;

	m_pState->Update(deltaT);
	AdjustSpritesToFitDirection();

	if (m_pState->GetStateFinished())
	{
		AlienState* followUpState = m_pState->GetFollowUpState();
		delete m_pState;
		m_pState = followUpState;
	}

}

void AIFlyComponent::AdjustSpritesToFitDirection()
{
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	SDL_Rect srcRect = renderComp->GetSrcRect();
	int initialOffsetWidth = 1;
	int offsetWidth = 2;
	int spriteIndex = int(std::round(m_RotationRadians / float(M_PI / 8.0)));

	spriteIndex = spriteIndex % 16;

	srcRect.x = initialOffsetWidth + (spriteIndex * srcRect.w) + (spriteIndex * offsetWidth);
	renderComp->SetSrcRect(srcRect);
}