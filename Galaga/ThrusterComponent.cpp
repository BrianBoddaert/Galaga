#include "stdafx.h"
#include "ThrusterComponent.h"
#include "TransformComponent.h"

using namespace Willem;

const float ThrusterComponent::m_ProjectileSpeed{550};

ThrusterComponent::ThrusterComponent(const Willem::Vector2& dir)
	:m_Direction{dir}
{

}
void ThrusterComponent::Update(float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	if (!transform)
		return;

	transform->SetPosition(transform->GetPosition() + m_Direction * m_ProjectileSpeed * deltaT);

}