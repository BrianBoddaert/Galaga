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
	//TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	//const Vector3& pos = transform->GetPosition();

	//SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
	//SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	//const SDL_Surface* surface = Minigin::GetWindowSurface();

	//// Set initial location
	//transform->SetPosition({ surface->w / 2.0f - halfSize.w,0,0 });


	//float borderOffset = 100.0f;
	//const Vector2 destination  = { float(surface->w - halfSize.w - borderOffset), float(surface->h / 2.0f - halfSize.h) };
	//const Vector2 direction = (destination - pos).Normalize(); 
	//m_SubStage = new DSS_MoveToPoint(m_pGameObject,this, destination,direction);

}
void SpawnDiveState::Exit() 
{
}

AlienState* SpawnDiveState::GetFollowUpState() const
{
	return new FormationState(m_pGameObject);
}