#include "stdafx.h"
#include "FormationState.h"
#include "BombRunState.h"
#include <SDL.h>
#include "EnemyManager.h"
#include "TransformComponent.h"
#include "GameObject.h"

using namespace Willem;

FormationState::FormationState(Willem::GameObject* go) noexcept
	:AlienState{ go }
{
	Enter();
}

FormationState::~FormationState()
{
	Exit();
}


void FormationState::Update(float)
{

}

AlienState* FormationState::GetFollowUpState() const
{
	return new BombRunState(m_pGameObject);
}

void FormationState::Enter()
{
	m_RotationRadians = float(M_PI);
	AdjustSpritesToFitDirection();

	const Vector2& desiredPosition = EnemyManager::GetInstance().GetBeeFormationPosition(m_pGameObject);
	m_pGameObject->GetComponent<TransformComponent>()->SetPosition(desiredPosition);
	
}

void FormationState::Exit() {}