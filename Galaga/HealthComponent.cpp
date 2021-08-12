#include "stdafx.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"
#include "CollisionManager.h"

#include "RenderComponent.h"
#include "AIFlyComponent.h"
#include "ExplosionManager.h"
#include "ControlComponent.h"
#include "GameObject.h"
#include "BombRunState.h"
#include "ShootComponent.h"
#include <string>
#include "AudioClasses.h"

using namespace Willem;

HealthComponent::HealthComponent(int health, bool removeWhenOutOfScreen)
	: m_HealthPoints{ health }
	, m_MaxHealthPoints{ health }
	, m_RemoveWhenOutOfScreen{ removeWhenOutOfScreen }
{}

void HealthComponent::Update(float)
{
	if (!m_RemoveWhenOutOfScreen)
		return;

	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	const Vector3& pos = transform->GetPosition();
	const SDL_Surface* surface = Minigin::GetWindowSurface();

	if (!transform)
		return;

	if (pos.x < 0 || pos.y < 0 || pos.x > surface->w || pos.y > surface->h)
		Die();

}

const int HealthComponent::GetHealthPoints() const
{
	return m_HealthPoints;
}

void HealthComponent::Hit(int amount)
{
	m_HealthPoints -= amount;

	if (m_pGameObject->HasTag("Alien"))
	{
		ServiceLocator::GetSoundSystem().QueueSound("EnemyDies", false, 0.3f);
	}
	else if (m_pGameObject->HasTag("Player"))
	{
		ServiceLocator::GetSoundSystem().QueueSound("PlayerDies", false, 0.3f);
	}

	if (m_HealthPoints <= 0)
		Die();
	else if (m_pGameObject->HasTag("Boss"))
	{
		RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
		SDL_Rect srcRect = renderComp->GetSrcRect();
		const int offsetY = 2;
		srcRect.y += srcRect.h * 2 + offsetY * 2;

		AIFlyComponent* flyComp = m_pGameObject->GetComponent<AIFlyComponent>();
		flyComp->SetUpperSrcRectYPos(flyComp->GetUpperSrcRectYPos() + (srcRect.h * 2 + offsetY * 2));

		renderComp->SetSrcRect(srcRect);
	}
	else if (m_pGameObject->HasTag("Player"))
	{
		auto transformComp = m_pGameObject->GetComponent<TransformComponent>();
		ExplosionManager::GetInstance().SpawnExplosion(transformComp->GetPosition());

		auto controlComp = m_pGameObject->GetComponent<ControlComponent>();
		transformComp->SetPosition(controlComp->GetSpawnPosition());
		controlComp->DisableCaughtInTractorBeam();
		m_pGameObject->GetComponent<ShootComponent>()->SetEnabled(true);

		std::string name;
		if (m_pGameObject->HasTag("Player1"))
			name = "LifeCounter" + std::to_string(m_HealthPoints);
		else
			name = "LifeCounter" + std::to_string(m_HealthPoints+2);
		SceneManager::GetInstance().GetCurrentScene()->RemoveObjectsByName(name);
	}

}
void HealthComponent::Heal(int amount)
{
	m_HealthPoints += amount;

	if (m_HealthPoints > m_MaxHealthPoints)
		m_HealthPoints = m_MaxHealthPoints;
}

void HealthComponent::Die()
{

	if (!m_pGameObject->HasTag("Bullet"))
	ExplosionManager::GetInstance().SpawnExplosion(m_pGameObject->GetComponent<TransformComponent>()->GetPosition());

	auto scene = Willem::SceneManager::GetInstance().GetCurrentScene();
	scene->RemoveObjectsByObject(m_pGameObject);

	if (m_pGameObject->HasTag("Player"))
		m_pGameObject->Notify("PlayerDied");

	CollisionManager::GetInstance().RemoveColliderByObject(m_pGameObject);	

}
