#include "stdafx.h"
#include "TractorBeamDive.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

#include "ShootComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include <cmath>
#include "DSS_TractorBeam.h"

using namespace Willem;

TractorBeamDive::TractorBeamDive(Willem::GameObject* go, bool diveToTheRight)
	:Dive{ go }
	, m_DiveToTheRight{ diveToTheRight }
{
	Enter();
}

TractorBeamDive::~TractorBeamDive()
{
	Exit();
}

Willem::Vector2 TractorBeamDive::GetClosestPlayerPos() const
{
	const Vector2 alienPos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	const auto& players = SceneManager::GetInstance().GetCurrentScene()->GetPlayers();

	if (players.empty())
	{
		std::cout << "WARNING: GetClosestPlayer returned no player found" << std::endl;
		return Vector2(0, 0);
	}

	float closestDistance = FLT_MAX;
	int closestPlayerIndex = -1;

	for (size_t i = 0; i < players.size(); i++)
	{
		const Vector2 playerPos = players[i]->GetComponent<TransformComponent>()->GetPosition();
		const float distance = (playerPos - alienPos).Magnitude();
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestPlayerIndex = int(i);
		}
	}

	if (closestPlayerIndex == -1)
	{
		std::cout << "WARNING: Closest player not found!" << std::endl;
		return Vector2(0, 0);
	}

	return players[closestPlayerIndex]->GetComponent<TransformComponent>()->GetPosition();

}


void TractorBeamDive::Update(float deltaT)
{
	m_pState->Update(deltaT);

	if (!m_pState->GetStateFinished())
		return;

	delete m_pState;
	m_pState = nullptr;

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();

	float offsetFromSide = 150;
	const Vector2 tractorPosRight = { float(surface->w - offsetFromSide),float(surface->h - 200) };
	const Vector2 tractorPosLeft = { float(offsetFromSide), float(surface->h - 200) };

	switch (m_DiveProcess)
	{
	case 0:
	{
		Vector2 destination;
		if (m_DiveToTheRight)
			destination = { tractorPosRight.x,tractorPosRight.y };
		else
			destination = { tractorPosLeft.x,tractorPosLeft.y };


		const Vector2 direction = (destination - pos).Normalize();
		m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, 0.6f);

	}
	break;
	case 1:
	{
		m_pState = new DSS_TractorBeam(m_pGameObject);

	}
	break;
	case 2:
	{
		FindFormationPosAndMoveToIt();
	}
	break;
	case 3:
	{
		m_Completed = true;
	}
	break;
	}
	m_DiveProcess++;
}

void TractorBeamDive::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();

	if (m_DiveToTheRight)
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, 0, float(-M_PI * 1.5), false, Vector2{ -25,0 });
	else
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI), float(M_PI / 2), true, Vector2{ 25,0 });
}

void TractorBeamDive::Exit()
{
	if (m_pState)
		delete m_pState;
}