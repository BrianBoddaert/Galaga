#include "stdafx.h"
#include "ShootComponent.h"
#include "GameObject.h"
#include <memory>
#include <SDL.h>
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "CollisionManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "AudioClasses.h"
#include "ThrusterComponent.h"
#include "HealthComponent.h"

using namespace Willem;

void ShootComponent::Fire(const Vector2& dir)
{
	if (m_pGameObject->HasTag("Player") && m_Bullets.size() >= 2)
		return;

	//313x 140
	//3x 8

	auto bullet = std::make_shared<GameObject>("Bullet");

	SDL_Rect srcRect = { 313,140,3,8 };
	const Vector2 playerHalfSize = { srcRect.w / 2.0f,srcRect.h / 2.0f };
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3 spawnPos = { transform->GetPosition() + Vector3(playerHalfSize.x,0,0) };

	bullet->AddComponent(new RenderComponent(srcRect));
	bullet->SetTexture("Galaga2.png");
	bullet->AddComponent(new TransformComponent(spawnPos, 2.0f));
	bullet->AddComponent(new ThrusterComponent(dir));
	bullet->AddComponent(new HealthComponent(1,true));
	bullet->AddTag("PlayerBullet");

	CollisionManager::GetInstance().AddCollider(bullet);
	SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
	m_Bullets.push_back(bullet);
	ServiceLocator::GetSoundSystem().QueueSound(EffectId::Jump, 1.0f);
}

void ShootComponent::Update(float)
{
	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i].use_count() <= 1)
			m_Bullets.erase(m_Bullets.begin() + i);
	}
}