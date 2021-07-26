#include "stdafx.h"
#include "ControlComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "RenderComponent.h"

using namespace Willem;

ControlComponent::ControlComponent(const Willem::Vector3& spawn)
	:m_SpawnPosition{ spawn }
	,m_Speed{200}
{

	Initialize();
}

void ControlComponent::Initialize()
{

}


void ControlComponent::SetMoveInput(const MoveInputDirections& dir, bool on)
{
	m_MoveInputsActive[int(dir)] = on;
}

void ControlComponent::Update(float deltaT)
{
	if (!m_pGameObject || InputManager::GetInstance().GetInputLocked())
		return;

	if (m_MoveInputsActive[(int)MoveInputDirections::Left])
		Move(false, deltaT);
	else if (m_MoveInputsActive[(int)MoveInputDirections::Right])
		Move(true, deltaT);
	else
		return;

}

void ControlComponent::Move(bool right,float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	auto windowSurface = Minigin::GetWindowSurface();

	if (right)
	{
		if (transform->GetPosition().x + renderComp->GetSpritePixelSizeScaled().x >= windowSurface->w)
			return;

		transform->SetPosition(transform->GetPosition() + Vector3{ m_Speed * deltaT,0,0 });
	}
	else
	{
		if (transform->GetPosition().x <= 0)
			return;

		transform->SetPosition(transform->GetPosition() + Vector3{ -m_Speed * deltaT,0,0 });
	}




}