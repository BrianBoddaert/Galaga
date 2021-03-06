#include "stdafx.h"
#include "Dive_UpToRight.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

using namespace Willem;

Dive_UpToRight::Dive_UpToRight(Willem::GameObject* go)
	:Dive{ go }
{
	Enter();
}

Dive_UpToRight::~Dive_UpToRight()
{
	Exit();
}

void Dive_UpToRight::Update(float deltaT)
{
	m_pState->Update(deltaT);

	if (m_pState->GetStateFinished())
	{
		TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
		const Vector3& pos = transform->GetPosition();

		delete m_pState;
		m_pState = nullptr;

		switch (m_DiveProcess)
		{
		case 0:
		{
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(-M_PI / 2.0), float(M_PI - M_PI / 5.0));
		}
		break;
		case 1:
		{
			FindFormationPosAndMoveToIt();
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

void Dive_UpToRight::Enter()
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

void Dive_UpToRight::Exit()
{
	if (m_pState)
		delete m_pState;
}