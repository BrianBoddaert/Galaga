#include "stdafx.h"
#include "ControlComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Minigin.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "EnemyManager.h"
#include "AIFlyComponent.h"

using namespace Willem;

ControlComponent::ControlComponent(const Willem::Vector3& spawn)
	:m_SpawnPosition{ spawn }
	, m_Speed{200}
	, m_Enabled{true}
	, m_CaughtInTractorBeam{false}
	, m_TractorBeamSuckSpeed{150.0f}
	, m_CapturerLocation{0,0}
	, m_CapturerDirection{ 0,0 }
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
	if (!m_pGameObject)
		return;

	if (m_CaughtInTractorBeam)
		if (m_EscapeFromTractorBeam)
			TractorBeamRelease(deltaT);
		else
			TractorBeamImprison(deltaT);

	 if( InputManager::GetInstance().GetInputLocked() || !m_Enabled)
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

void ControlComponent::TractorBeamImprison(float deltaT)
{
	if (m_pCapturer.use_count() <= 1)
	{
		m_EscapeFromTractorBeam = true;
		return;
	}

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	transform->SetPosition(transform->GetPosition() + m_CapturerDirection * m_TractorBeamSuckSpeed * deltaT);

	const float minimalOffset = 0.01f * m_TractorBeamSuckSpeed;

	if ((transform->GetPosition() - m_CapturerLocation).Magnitude() <= minimalOffset)
	{
		// Spawn enemy fighterJet
	
		m_pCapturer.lock()->GetComponent<AIFlyComponent>()->SetCapturedPlayer(EnemyManager::GetInstance().SpawnCapturedPlayer(transform->GetPosition(), m_pCapturer));
		// Player dies
		m_pGameObject->GetComponent<HealthComponent>()->Hit();
		// Back to normal behavior
		m_CaughtInTractorBeam = false;
		m_Enabled = true;
	}

}

void ControlComponent::TractorBeamRelease(float deltaT)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	transform->SetPosition(transform->GetPosition() - m_CapturerDirection * m_TractorBeamSuckSpeed * deltaT);

	const float minimalOffset = 0.01f * m_TractorBeamSuckSpeed;

	if (abs(transform->GetPosition().y - m_SpawnPosition.y) <= minimalOffset)
	{
		// Back to normal behavior
		m_CaughtInTractorBeam = false;
		m_Enabled = true;
		m_EscapeFromTractorBeam = false;
	}

}

void ControlComponent::EnableCaughtInTractorBeam(std::weak_ptr<Willem::GameObject> boss)
{
	m_CaughtInTractorBeam = true;
	m_pCapturer = boss;
	//float offsetOnSprite = 16;
	m_CapturerLocation = boss.lock()->GetComponent<TransformComponent>()->GetPosition() + Vector2(0,float( boss.lock()->GetComponent<RenderComponent>()->GetSrcRect().h * GAMESCALE ));/* + Vector2(offsetOnSprite * GAMESCALE, 0);*/

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	m_CapturerDirection = (m_CapturerLocation - transform->GetPosition()).Normalize();
}