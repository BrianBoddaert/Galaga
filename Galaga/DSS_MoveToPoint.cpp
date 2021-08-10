#include "stdafx.h"
#include "DSS_MoveToPoint.h"
#include "TransformComponent.h"
#include "AIFlyComponent.h"
using namespace Willem;

DSS_MoveToPoint::DSS_MoveToPoint(Willem::GameObject* go, const Willem::Vector2& pos, const Willem::Vector2& dir, float speedMultiplier) noexcept
	:DiveSubState{ go }
	,m_Destination{pos}
	,m_Direction{dir}
	,m_SpeedMultiplier{ speedMultiplier }
{
	Enter();
}

DSS_MoveToPoint::~DSS_MoveToPoint()
{
	Exit();
}
void DSS_MoveToPoint::Update(float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	auto flyComp = m_pGameObject->GetComponent<AIFlyComponent>();
	float speed = flyComp->GetSpeed() * m_SpeedMultiplier;
	Vector2 distanceTravelled = m_Direction * (speed * deltaT);
	transform->SetPosition(pos + distanceTravelled);

	flyComp->SetRotationRadians(m_Direction.DirectionToLocalOrientationAngle());

	// Finished condition
	const float minimalOffset = 0.02f * speed;
	if ((pos - m_Destination).Magnitude() <= minimalOffset)
		m_SubStateFinished = true;

}


void DSS_MoveToPoint::SetDestinationAndCalculateDirection(const Willem::Vector2& dest)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();

	m_Destination = dest;
	m_Direction = (dest - pos).Normalize();
}
void DSS_MoveToPoint::Enter()
{

}
void DSS_MoveToPoint::Exit()
{

}