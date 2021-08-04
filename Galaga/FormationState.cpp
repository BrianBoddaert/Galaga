#include "stdafx.h"
#include "FormationState.h"
#include "BombRunState.h"
#include <SDL.h>
#include "EnemyManager.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "AIFlyComponent.h"

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
	return nullptr;
}

void FormationState::Enter()
{
	m_pGameObject->GetComponent<AIFlyComponent>()->SetRotationRadians(float(M_PI));
	Vector2 desiredPosition;

	if (m_pGameObject->HasTag("Bee"))
		desiredPosition = EnemyManager::GetInstance().GetBeeFormationPosition(m_pGameObject);
	else if (m_pGameObject->HasTag("Butterfly"))
		desiredPosition = EnemyManager::GetInstance().GetButterflyFormationPosition(m_pGameObject);
	else if (m_pGameObject->HasTag("Boss"))
		desiredPosition = EnemyManager::GetInstance().GetBossFormationPosition(m_pGameObject);

	m_pGameObject->GetComponent<TransformComponent>()->SetPosition(desiredPosition);
	
}

void FormationState::Exit() {}