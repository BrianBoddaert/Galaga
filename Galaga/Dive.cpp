#include "stdafx.h"
#include "Dive.h"
#include "TransformComponent.h"
#include "Structs.h"
#include "EnemyManager.h"
#include "DSS_MoveToPoint.h"

using namespace Willem;

void Dive::ReserveSpotAndMoveToIt()
{
	// Reserve a spot in the formation, calculate the direction to this point
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	EnemyManager& enemyManager = EnemyManager::GetInstance();
	Vector2 destination;
	if (m_pGameObject->HasTag("Bee"))
	{
		enemyManager.ClaimSpotInBeeFormation(m_pGameObject);
		destination = enemyManager.GetBeeFormationPosition(m_pGameObject);
	}
	else if (m_pGameObject->HasTag("Butterfly"))
	{
		enemyManager.ClaimSpotInButterflyFormation(m_pGameObject);
		destination = enemyManager.GetButterflyFormationPosition(m_pGameObject);
	}
	else if (m_pGameObject->HasTag("Boss"))
	{
		enemyManager.ClaimSpotInBossFormation(m_pGameObject);
		destination = enemyManager.GetBossFormationPosition(m_pGameObject);
	}

	Vector2 direction = (destination - pos).Normalize();
	m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);

}