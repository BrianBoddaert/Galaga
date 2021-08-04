#include "stdafx.h"
#include "ButterflyDive.h"

#include "DSS_MoveToPoint.h"
#include "DSS_CircleAroundPoint.h"

#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"

using namespace Willem;

ButterflyDive::ButterflyDive(Willem::GameObject* go, bool diveToTheRight)
	:Dive{ go }
	, m_DiveToTheRight{ diveToTheRight }
{
	Enter();
}

ButterflyDive::~ButterflyDive()
{
	Exit();
}

void ButterflyDive::Update(float deltaT)
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
			const Vector2 destination = { float(surface->w - xBorderOffset), beeDiveDownYPos };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);

		}
		break;
		case 1:
		{
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI / 2), float(M_PI / 4), true, Vector2{ 0,15 });
		}
		break;
		case 2:
		{
			const Vector2 destination = { float(surface->w - xBorderOffset),  float(surface->h - yBorderOffset) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
		}
		break;
		case 3:
		{
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, -float(M_PI/3), float(M_PI * 0.75), true, Vector2{ -25,0 });
		}
		break;
		case 4:
		{
			const Vector2 destination = { float(surface->w/2), float(surface->h - halfSize.h* GAMESCALE) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
		}
		break;
		case 5:
		{
			const Vector2 destination = { xBorderOffset, float(surface->h - yBorderOffset) };
			const Vector2 direction = (destination - pos).Normalize();
			m_pState = new DSS_MoveToPoint(m_pGameObject, destination, direction);
		}
		break;
		case 6:
		{
			m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, float(M_PI/8), float(M_PI * 1.5), true, Vector2{ 0,-25 });
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

void ButterflyDive::Enter()
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();

	m_pState = new DSS_CircleAroundPoint(m_pGameObject, pos, 0, float(-M_PI * 1.5), false, Vector2{-25,0});
}

void ButterflyDive::Exit()
{
	if (m_pState)
		delete m_pState;
}