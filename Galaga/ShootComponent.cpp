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
	if (!m_Enabled)
		return;

	if (m_pGameObject->HasTag("Player"))
	{
		if (m_Bullets.size() >= 2)
			return;
	//313x 140
	//3x 8

	auto bullet = std::make_shared<GameObject>("Bullet");

	SDL_Rect srcRect = { 313,122,3,8 };
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	const SDL_Rect& playerSrcRect = renderComp->GetSrcRect();
	const Vector2 playerHalfSize = { playerSrcRect.w / 2.0f ,playerSrcRect.h / 2.0f };

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();

	const Vector3 spawnPos = { transform->GetPosition() + Vector3(playerHalfSize.x * GAMESCALE - (srcRect.w/2*GAMESCALE),0,0) };

	bullet->AddComponent(new RenderComponent(srcRect));
	bullet->SetTexture("Galaga2.png");
	bullet->AddComponent(new TransformComponent(spawnPos, GAMESCALE));
	bullet->AddComponent(new ThrusterComponent(dir));
	bullet->AddComponent(new HealthComponent(1,true));
	bullet->AddTag("PlayerBullet");
	bullet->AddTag("Bullet");
	bullet->SetParent(SceneManager::GetInstance().GetCurrentScene()->GetObjectByObject(m_pGameObject));

	CollisionManager::GetInstance().AddCollider(bullet);
	SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
	m_Bullets.push_back(bullet);
	ServiceLocator::GetSoundSystem().QueueSound("PlayerShoot",false, 0.3f);
	}
	else
	{
		auto bullet = std::make_shared<GameObject>("Bullet");

		SDL_Rect srcRect = { 313,140,3,8 };
		RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
		const SDL_Rect& alienSrcRect = renderComp->GetSrcRect();
		const Vector2 alienHalfSize = { alienSrcRect.w / 2.0f ,alienSrcRect.h / 2.0f };

		TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();

		const Vector3 spawnPos = { transform->GetPosition() + Vector3(alienHalfSize.x * GAMESCALE - (srcRect.w / 2 * GAMESCALE),0,0) };

		bullet->AddComponent(new RenderComponent(srcRect));
		bullet->SetTexture("Galaga2.png");
		bullet->AddComponent(new TransformComponent(spawnPos, GAMESCALE));
		bullet->AddComponent(new ThrusterComponent(dir));
		bullet->AddComponent(new HealthComponent(1, true));
		bullet->AddTag("AlienBullet");
		bullet->AddTag("Bullet");
		bullet->SetParent(SceneManager::GetInstance().GetCurrentScene()->GetObjectByObject(m_pGameObject));

		CollisionManager::GetInstance().AddCollider(bullet);
		SceneManager::GetInstance().GetCurrentScene()->Add(bullet);
		m_Bullets.push_back(bullet);
	}


}

void ShootComponent::DoubleFire(const Vector2& dir)
{
	Fire(dir);
	m_DoubleFireDir = dir;
	m_DoubleFireActive = true;
}
void ShootComponent::Update(float deltaT)
{
	for (size_t i = 0; i < m_Bullets.size(); i++)
	{
		if (m_Bullets[i].use_count() <= 1)
			m_Bullets.erase(m_Bullets.begin() + i);
	}

	if (!m_DoubleFireActive)
		return;

	m_DoubleFireCounter += deltaT;
	if (m_DoubleFireCounter >= m_DoubleFireInterval)
	{
		Fire(m_DoubleFireDir);
		m_DoubleFireCounter = 0.0f;
		m_DoubleFireActive = false;
	}
}