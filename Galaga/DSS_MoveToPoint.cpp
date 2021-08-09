#include "stdafx.h"
#include "DSS_MoveToPoint.h"
#include "TransformComponent.h"
#include "AIFlyComponent.h"
using namespace Willem;

DSS_MoveToPoint::DSS_MoveToPoint(Willem::GameObject* go, const Willem::Vector2& pos, const Willem::Vector2& dir, bool introDive) noexcept
	:DiveSubState{ go }
	,m_Destination{pos}
	,m_Direction{dir}
	, m_IntroDive{ introDive }
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
	float speed;
	if (m_IntroDive)
		speed = flyComp->GetIntroDiveSpeed();
	else
		speed = flyComp->GetBombRunSpeed();

	Vector2 distanceTravelled = m_Direction * (speed * deltaT);
	transform->SetPosition(pos + distanceTravelled);

	flyComp->SetRotationRadians(m_Direction.DirectionToLocalOrientationAngle());

	// Finished condition
	const float minimalOffset = 0.02f * speed;
	if ((pos - m_Destination).Magnitude() <= minimalOffset)
		m_SubStateFinished = true;

}


void DSS_MoveToPoint::Enter()
{

}
void DSS_MoveToPoint::Exit()
{

}