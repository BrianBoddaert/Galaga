#include "stdafx.h"
#include "AIFlyComponent.h"
#include "TransformComponent.h"
#include "Minigin.h"
#include "SpawnDiveState.h"
#include "EnemyManager.h"
#include <cmath>

using namespace Willem;


AIFlyComponent::AIFlyComponent(Willem::GameObject* go, SpawnDiveState* state, int srcRectYPos)
	:Component{ go }
	,m_pState{ state }
	, m_Speed{ 400.0f }
	, m_RotationSpeed{9} 
	, m_RotationRadians{float(M_PI)}
	, m_UpperSrcRectYPos{ srcRectYPos }
{
	m_pRenderComponent = m_pGameObject->GetComponent<Willem::RenderComponent>();
}

void AIFlyComponent::SetState(AlienState* state)
{
	if (m_pState)
	delete m_pState;

	m_pState = state;
}

AIFlyComponent::~AIFlyComponent()
{
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
	int initialOffsetWidth = 1;
	int offsetWidth = 2;
	int highestSprite = 15;
	int spriteIndex;

	if (m_RotationRadians > 0.0f)
		spriteIndex = int(std::round(m_RotationRadians / float(M_PI / 8.0)));
	else
	{
		spriteIndex = highestSprite - int(std::round(std::fmod(abs(m_RotationRadians),float(M_PI*2)) / float(M_PI / 8.0)));
	}
		
	spriteIndex = spriteIndex % 16;

	SDL_Rect srcRect = m_pRenderComponent->GetSrcRect();
	srcRect.x = initialOffsetWidth + (spriteIndex * srcRect.w) + (spriteIndex * offsetWidth);
	m_pRenderComponent->SetSrcRect(srcRect);
}