#include "stdafx.h"
#include "DSS_TractorBeam.h"
#include "TransformComponent.h"
#include "AIFlyComponent.h"

#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "AudioClasses.h"

#include "HealthComponent.h"

using namespace Willem;

DSS_TractorBeam::DSS_TractorBeam(Willem::GameObject* go) noexcept
	:DiveSubState{ go }
	, m_TractorBeamYSpriteInterval{0.3f}
	, m_TractorBeamYSpriteTimer{0.0f}
	, m_TractorBeamXSpriteInterval{ 0.15f }
	, m_TractorBeamXSpriteTimer{ 0.0f }
	, m_TractorBeamActiveDuration{1.0f}
	, m_TractorBeamActiveTimer{0.0f}
	, m_TractorBeamExpanding{ true }
	, m_TractorBeamActive{false}
{
	Enter();
}

DSS_TractorBeam::~DSS_TractorBeam()
{
	Exit();
}
void DSS_TractorBeam::Update(float deltaT)
{
	m_TractorBeamYSpriteTimer += deltaT;
	m_TractorBeamXSpriteTimer += deltaT;

	if (m_TractorBeamActive && m_TractorBeamExpanding)
	{
		m_TractorBeamActiveTimer += deltaT;
		if (m_TractorBeamActiveTimer > m_TractorBeamActiveDuration)
			m_TractorBeamExpanding = false;
	}
		

	auto flyComp = m_pGameObject->GetComponent<AIFlyComponent>();
	flyComp->SetRotationRadians(Vector2(0,1).DirectionToLocalOrientationAngle());

	if (m_TractorBeamXSpriteTimer >= m_TractorBeamXSpriteInterval)
	{				 
		m_TractorBeamXSpriteTimer = 0.0f;

		auto renderComp = m_pTractorBeam.lock()->GetComponent<RenderComponent>();
		auto srcRect = renderComp->GetSrcRect();
		int offset = 2;

		if (srcRect.x != 101)
			srcRect.x += srcRect.w + offset;
		else
			srcRect.x = 1;
		renderComp->SetSrcRect(srcRect);
	}

	if (m_TractorBeamYSpriteTimer >= m_TractorBeamYSpriteInterval)
	{
		m_TractorBeamYSpriteTimer = 0.0f;

		auto renderComp = m_pTractorBeam.lock()->GetComponent<RenderComponent>();
		auto srcRect = renderComp->GetSrcRect();
		int offset = 2;

		if (m_TractorBeamExpanding)
		{
			if (srcRect.y != 411)
				srcRect.y += srcRect.h + offset;
			else
				m_TractorBeamActive = true;
		}
		else
		{
			if (srcRect.y != 1)
				srcRect.y -= srcRect.h + offset;
			else
			{
				SceneManager::GetInstance().GetCurrentScene()->RemoveObjectsByObject(m_pTractorBeam.lock().get());
				m_SubStateFinished = true;
			}
		}


		renderComp->SetSrcRect(srcRect);
	}

}


void DSS_TractorBeam::Enter()
{
	auto beam = std::make_shared<Willem::GameObject>("TractorBeam");

	SDL_Rect srcRect = { 1,1,48,80 };
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	const SDL_Rect& playerSrcRect = renderComp->GetSrcRect();
	const Vector2 playerHalfSize = { playerSrcRect.w / 2.0f ,playerSrcRect.h / 2.0f };

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();

	const Vector3 spawnPos = { transform->GetPosition() + Vector3(playerHalfSize.x * GAMESCALE - (srcRect.w / 2 * GAMESCALE),float(playerSrcRect.h * GAMESCALE),0) };

	beam->AddComponent(new RenderComponent(srcRect));
	beam->SetTexture("TractorBeam.png");
	beam->AddComponent(new TransformComponent(spawnPos, GAMESCALE));
	beam->AddTag("TractorBeam");
	beam->AddTag("Alien");

	beam->SetParent(SceneManager::GetInstance().GetCurrentScene()->GetObjectByObject(m_pGameObject));
	CollisionManager::GetInstance().AddCollider(beam);
	SceneManager::GetInstance().GetCurrentScene()->Add(beam);
	ServiceLocator::GetSoundSystem().QueueSound(EffectId::Jump, 1.0f);
	m_pTractorBeam = beam;
}
void DSS_TractorBeam::Exit()
{
	HealthComponent* healthComp = m_pGameObject->GetComponent<HealthComponent>();

	if (healthComp && healthComp->GetHealthPoints() <= 0 && m_pTractorBeam.use_count() >= 1)
		SceneManager::GetInstance().GetCurrentScene()->RemoveObjectsByObject(m_pTractorBeam.lock().get());
}