#include "stdafx.h"
#include "BeeDive.h"

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

using namespace Willem;

BeeDive::BeeDive(Willem::GameObject* go, bool diveToTheRight)
	:Dive{ go }
	, m_DiveToTheRight{ diveToTheRight }
{
	Enter();
}

BeeDive::~BeeDive()
{
	Exit();
}

Willem::Vector2 BeeDive::GetClosestPlayerPos() const
{
	const Vector2 alienPos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	const auto& players = SceneManager::GetInstance().GetCurrentScene()->GetPlayers();

	if (players.empty())
	{
		std::cout << "WARNING: GetClosestPlayer returned no player found" << std::endl;
		return Vector2(0,0);
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


void BeeDive::Update(float deltaT)
{
	m_pState->Update(deltaT);

	if (m_pState->GetStateFinished())
	{
		delete m_pState;
		m_pState = nullptr;

		TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
		const Vector3& pos = transform->GetPosition();
		SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
		SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
		const SDL_Surface* surface = Minigin::GetWindowSurface();
		float xBorderOffset = 100.0f;
		float yBorderOffset = 50.0f;

		switch (m_DiveProcess)
		{
		case 0:
		{

			float beeDiveDownYPos = 200.0f;
			Vector2 destination;
			if (m_DiveToTheRight)
				destination = { float(surface->w - xBorderOffset), beeDiveDownYPos };
			else
				destination = { xBorderOffset, beeDiveDownYPos };

			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, false);

			// Fire
			m_pGameObject->GetComponent<ShootComponent>()->DoubleFire((GetClosestPlayerPos() - pos).Normalize());
		}
		break;
		case 1:
		{
			if (m_DiveToTheRight)
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 2), float(M_PI / 4), true, Vector2{ 0,15 });
			else
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 2), -float(M_PI *1.25), false, Vector2{ 0,15 });

		}
		break;
		case 2:
		{
			Vector2 destination;
			if (m_DiveToTheRight)
				destination = { float(surface->w - xBorderOffset),  float(surface->h - yBorderOffset) };
			else
				destination = {  xBorderOffset,  float(surface->h - yBorderOffset) };

			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction,false);
		}
		break;
		case 3:
		{
			if (m_DiveToTheRight)
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 3), float(M_PI * 0.75), true, Vector2{ -25,0 });
			else
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI), -float(M_PI * 1.75), false, Vector2{ 25,0 });
		}
		break;
		case 4:
		{
			const Vector2 destination = { float(surface->w / 2), float(surface->h - halfSize.h * GAMESCALE) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, false);
		}
		break;
		case 5:
		{
			Vector2 destination;
			if (m_DiveToTheRight)
				destination = { xBorderOffset, float(surface->h - yBorderOffset) };
			else
				destination = { float(surface->w - xBorderOffset),  float(surface->h - yBorderOffset) };

			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction, false);
		}
		break;
		case 6:
		{
			if (m_DiveToTheRight)
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(M_PI / 2), float(M_PI * 1.5), true, Vector2{ 0,-25 });
			else
				m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(M_PI / 2), -float(M_PI / 2), false, Vector2{ 0,-25 });
		}
		break;
		case 7:
		{
			FindFormationPosAndMoveToIt();
		}
		break;
		case 8:
		{
			m_Completed = true;
		}
		break;
		}


		m_DiveProcess++;
	}
}

void BeeDive::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();

	if (m_DiveToTheRight)
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, 0, float(-M_PI * 1.5), false, Vector2{ -25,0 });
	else
		m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(M_PI * 1.5), float(M_PI * 3), true, Vector2{ 25,0 });
}

void BeeDive::Exit()
{
	if (m_pState)
		delete m_pState;
}