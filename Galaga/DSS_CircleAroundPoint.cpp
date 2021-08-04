#include "stdafx.h"
#include "DSS_CircleAroundPoint.h"
#include "TransformComponent.h"
#include "AIFlyComponent.h"
using namespace Willem;

DSS_CircleAroundPoint::DSS_CircleAroundPoint(Willem::GameObject* go, const Willem::Vector2& pos,float rot, float rotlimit,bool clockWise, const Vector2& offset) noexcept
	:DiveSubState{ go }
	, m_Center{ pos + offset }
	,m_Offset{ abs(offset.x) + abs(offset.y)}
	,m_Rotation{rot}
	,m_RotationLimit{rotlimit}
	, m_Clockwise{ clockWise }
{
	Enter();
}

DSS_CircleAroundPoint::~DSS_CircleAroundPoint()
{
	Exit();
}
void DSS_CircleAroundPoint::Update(float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	auto flyComp = m_pGameObject->GetComponent<AIFlyComponent>();
	float rotSpeed = flyComp->GetRotationSpeed();


	//M_PI - M_PI/5
	if (m_Clockwise)
	{
		m_Rotation += rotSpeed * deltaT;
		flyComp->SetRotationRadians(m_Rotation + float(M_PI));

		if (m_Rotation >= m_RotationLimit)
			m_SubStateFinished = true;
	}
	else
	{
		m_Rotation -= rotSpeed * deltaT;
		flyComp->SetRotationRadians(m_Rotation);

		if (m_Rotation <= m_RotationLimit)
				m_SubStateFinished = true;
	}

	transform->SetPosition(m_Center +
		Vector2(m_Offset * cos(m_Rotation), m_Offset * sin(m_Rotation)));

}


void DSS_CircleAroundPoint::Enter()
{

}
void DSS_CircleAroundPoint::Exit()
{

}