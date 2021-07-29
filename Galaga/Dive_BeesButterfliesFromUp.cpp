#include "stdafx.h"
#include "Dive_BeesButterfliesFromUp.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

using namespace Willem;

Dive_BeesButterfliesFromUp::Dive_BeesButterfliesFromUp(Willem::GameObject* go)
	:Dive{ go }
{
	Enter();
}

Dive_BeesButterfliesFromUp::~Dive_BeesButterfliesFromUp()
{
	Exit();
}

void Dive_BeesButterfliesFromUp::Update(float deltaT)
{
	m_pState->Update(deltaT);


	if (m_pState->GetStateFinished())
	{
		TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
		const Vector3& pos = transform->GetPosition();

		switch (m_DiveProcess)
		{
		case 0:
		{
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(-M_PI / 2.0), float(M_PI - M_PI / 5.0));
		}
		break;
		case 1:
		{
			// Reserve a spot in the formation, calculate the direction to this point
			EnemyManager& enemyManager = EnemyManager::GetInstance();
			enemyManager.ClaimSpotInBeeFormation(m_pGameObject);
			Vector2 destination = enemyManager.GetBeeFormationPosition(m_pGameObject);
			Vector2 direction = (destination - pos).Normalize();

			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
		}
		break;
		case 2:
		{
			m_Completed = true;
		}
		break;
		}


		m_DiveProcess++;
	}
}

void Dive_BeesButterfliesFromUp::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
	SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();

	float borderOffset = 100.0f;
	const Vector2 destination = { float(surface->w - halfSize.w - borderOffset), float(surface->h / 2.0f - halfSize.h) };
	const Vector2 direction = (destination - pos).Normalize();
	m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
}

void Dive_BeesButterfliesFromUp::Exit()
{

}