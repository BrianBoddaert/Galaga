#include "stdafx.h"
#include "SpawnDiveState.h"
#include "GameObject.h"
#include <iostream>
#include "TransformComponent.h"
#include <SDL.h>
#include "Minigin.h"
#include "RenderComponent.h"
#include "EnemyManager.h"
#include <cmath>
#include "FormationState.h"

using namespace Willem;

SpawnDiveState::SpawnDiveState(Willem::GameObject* go) noexcept
	:AlienState{go}
	, m_Speed{ 100 }
	, m_RotationSpeed{ float(M_PI) }
	, m_CircularTurnRadians{ -(M_PI / 2) }
	, m_Stage{ DiveStages::FlyDownDiagonally }
{
	Enter();
}

SpawnDiveState::~SpawnDiveState()
{
	Exit();
}

void SpawnDiveState::Update(float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3& pos = transform->GetPosition();

	if (m_Stage == DiveStages::FlyDownDiagonally)
	{
		Vector2 distanceTravelled = m_DestinationDirection * (m_Speed * deltaT);
		transform->SetPosition(pos + distanceTravelled);
		// The below two lines are done to revert the effects of the inversed y coordinate.
		float angle = atan2(m_DestinationDirection.y * -1, m_DestinationDirection.x);
		angle = (M_PI * 2) - abs(angle);
		// This is done because the angle 0 is set upwards instead of to the right.
		m_RotationRadians = angle + M_PI / 2;

		// Finished condition
		const float parkingMinimalOffset = 1.0f;
		if ((pos - m_DestinationPosition).Magnitude() <= parkingMinimalOffset)
		{
			m_Stage = DiveStages::FlyInCircle;
			m_CircularTurnCenter = pos + Vector2(0, 50);
		}


		//const float borderOffset = 100.0f;

		//// cos of 45 degrees calculates the length of both sides if the hypothenuse is length 1
		//float xyLength = cos(float(M_PI) / 4.0f) * (m_Speed * deltaT);
		//transform->SetPosition(pos + Vector2(xyLength, xyLength));
		//m_RotationRadians = float(M_PI - M_PI / 4.0);

		//if (pos.x >= surface->w - borderOffset)
		//{
		//	m_Stage = DiveStages::FlyInCircle;
		//	m_CircularTurnCenter = pos + Vector2(0, 50);
		//}
	}

	if (m_Stage == DiveStages::FlyInCircle)
	{
		const float offset = 50;

		transform->SetPosition(m_CircularTurnCenter +
			Vector2(offset * cos(m_CircularTurnRadians), offset * sin(m_CircularTurnRadians)));

		m_CircularTurnRadians += m_RotationSpeed * deltaT;
		//m_CircularTurnRadians = std::fmod(m_CircularTurnRadians,float(M_PI*2));
		m_RotationRadians = m_CircularTurnRadians + float(M_PI);
		//m_RotationRadians = std::fmod(m_RotationRadians, float(M_PI * 2));

		if (m_CircularTurnRadians >= M_PI - M_PI/5)
		{
			m_Stage = DiveStages::ReturnToFormation;

			// Reserve a spot in the formation, calculate the direction to this point
			EnemyManager& enemyManager = EnemyManager::GetInstance();
			enemyManager.ClaimSpotInBeeFormation(m_pGameObject);
			m_DestinationPosition = enemyManager.GetBeeFormationPosition(m_pGameObject);
			m_DestinationDirection = (m_DestinationPosition - pos).Normalize();
		}
	}

	if (m_Stage == DiveStages::ReturnToFormation)
	{
		Vector2 distanceTravelled = m_DestinationDirection * (m_Speed * deltaT);
		transform->SetPosition(pos + distanceTravelled);
		// The below two lines are done to revert the effects of the inversed y coordinate.
		float angle = atan2(m_DestinationDirection.y * -1, m_DestinationDirection.x);
		angle = (M_PI * 2) - abs(angle);
		// This is done because the angle 0 is set upwards instead of to the right.
		m_RotationRadians = angle + M_PI / 2;
		

		// Finished condition
		const float parkingMinimalOffset = 1.0f;
		if ((pos - m_DestinationPosition).Magnitude() <= parkingMinimalOffset)
			m_StateFinished = true;


	}

	AdjustSpritesToFitDirection();
}
void SpawnDiveState::Enter()
{

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();

	SDL_Rect srcRect = m_pGameObject->GetComponent<RenderComponent>()->GetSrcRect();
	SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();

	// Set initial location
	transform->SetPosition({ surface->w / 2.0f - halfSize.w,0,0 });
	m_Stage = DiveStages::FlyDownDiagonally;

	m_DestinationPosition = { float(surface->w - halfSize.w), float(surface->h / 2.0f - halfSize.h) };
	m_DestinationDirection = (m_DestinationPosition - pos).Normalize();
}
void SpawnDiveState::Exit() {}

AlienState* SpawnDiveState::GetFollowUpState() const
{
	return new FormationState(m_pGameObject);
}