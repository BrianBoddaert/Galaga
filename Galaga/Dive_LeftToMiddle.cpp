#include "stdafx.h"
#include "Dive_LeftToMiddle.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

using namespace Willem;

Dive_LeftToMiddle::Dive_LeftToMiddle(Willem::GameObject* go)
	:Dive{ go }
{
	Enter();
}

Dive_LeftToMiddle::~Dive_LeftToMiddle()
{
	Exit();
}

void Dive_LeftToMiddle::Update(float deltaT)
{
	m_pState->Update(deltaT);

	if (m_pState->GetStateFinished())
	{
		delete m_pState;
		m_pState = nullptr;

		TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
		const Vector3& pos = transform->GetPosition();

		switch (m_DiveProcess)
		{
		case 0:
		{
			const Vector3 center = { pos.x,pos.y - 100.0f,pos.z };
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, center, float(M_PI / 2), float(-M_PI * 1.5), false);
		}
		break;
		case 1:
		{
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector2 destination = { float(surface->w/2) , float(surface->h/2 + 100.0) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
		}
		break;
		case 2:
		{
			ReserveSpotAndMoveToIt();
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
}

void Dive_LeftToMiddle::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
	SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();

	const Vector2 destination = { float(surface->w / 4),float(surface->h * 0.8) };
	const Vector2 direction = (destination - pos).Normalize();
	m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
}

void Dive_LeftToMiddle::Exit()
{
	if (m_pState)
		delete m_pState;
}