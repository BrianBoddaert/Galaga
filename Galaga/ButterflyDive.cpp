#include "stdafx.h"
#include "ButterflyDive.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

#include "ShootComponent.h"


using namespace Willem;

ButterflyDive::ButterflyDive(Willem::GameObject* go, bool diveToTheRight)
	:Dive{ go }
	, m_DiveToTheRight{ diveToTheRight }
	, m_HeadingBack{false}
	, m_FirstCycle{true}
{
	Enter();
}

ButterflyDive::~ButterflyDive()
{
	Exit();
}

Willem::Vector2 ButterflyDive::GetClosestPlayerPos() const
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

	for (int i = 0; i < players.size(); i++)
	{
		const Vector2 playerPos = players[i]->GetComponent<TransformComponent>()->GetPosition();
		const float distance = (playerPos - alienPos).Magnitude();
		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestPlayerIndex = i;
		}
	}

	if (closestPlayerIndex == -1)
	{
		std::cout << "WARNING: Closest player not found!" << std::endl;
		return Vector2(0, 0);
	}

	return players[closestPlayerIndex]->GetComponent<TransformComponent>()->GetPosition();

}

void ButterflyDive::Update(float deltaT)
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

	const Vector2 distanceTravelledEachStroke = { 100,75 };

	// Fire
	if (m_FirstCycle)
	{
		m_pGameObject->GetComponent<ShootComponent>()->DoubleFire((GetClosestPlayerPos() - pos).Normalize());
		m_FirstCycle = false;
	}
	

	if (!m_HeadingBack)
	{
		if (pos.x + srcRect.w * GAMESCALE < 0 || pos.x > surface->w)
		{
			m_DiveProcess = 0;
			m_HeadingBack = true;

			const Vector2 destination = { pos.x,  float(-srcRect.h * GAMESCALE - distanceTravelledEachStroke.y) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, 1.2f);
		}
		else
		{
			switch (m_DiveProcess % 4)
			{
			case 0:
			{
				const Vector2 destination = { pos.x,  pos.y + distanceTravelledEachStroke.y };
				const Vector2 direction = (destination - pos).Normalize();
				m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, 0.6f);

			}
			break;
			case 1:
			{
				if (m_DiveToTheRight)
					m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(M_PI), float(M_PI / 2), false, Vector2{ 25,0 });
				else
					m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, 0, float(M_PI / 2), true, Vector2{ -25,0 });
			}
			break;
			case 2:
			{
				Vector2 destination;
				if (m_DiveToTheRight)
					destination = { pos.x + distanceTravelledEachStroke.x,  pos.y };
				else
					destination = { pos.x - distanceTravelledEachStroke.x,  pos.y };

				const Vector2 direction = (destination - pos).Normalize();
				m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, 0.6f);
			}
			break;
			case 3:
			{
				if (m_DiveToTheRight)
					m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 2), 0, true, Vector2{ 0,25 });
				else
					m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 2), -float(M_PI), false, Vector2{ 0,25 });
			}
			break;
			}

		}
	}	
	else
	{
		switch (m_DiveProcess)
		{
		case 1:
		{
			const Vector2 destination = { float(surface->w/2),  pos.y };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, 1.2f);
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
	}


	m_DiveProcess++;
}

void ButterflyDive::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	 
	if (m_DiveToTheRight)
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, 0, float(-M_PI * 1.5), false, Vector2{ -25,0 });
	else
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI /2), float(M_PI/2), true, Vector2{ 25,0 });
}

void ButterflyDive::Exit()
{
	if (m_pState)
		delete m_pState;
}