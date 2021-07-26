#include "stdafx.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"

using namespace Willem;

HealthComponent::HealthComponent(int health, bool removeWhenOutOfScreen)
	: m_HealthPoints{health}
	, m_MaxHealthPoints{health}
	, m_RemoveWhenOutOfScreen{removeWhenOutOfScreen}
{

}

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

	if (m_HealthPoints < 0)
		Die();

}
void HealthComponent::Heal(int amount)
{
	m_HealthPoints += amount;

	if (m_HealthPoints > m_MaxHealthPoints)
		m_HealthPoints = m_MaxHealthPoints;
}

void HealthComponent::Die()
{
	auto scene = Willem::SceneManager::GetInstance().GetCurrentScene();
	scene->RemoveObjectsByObject(m_pGameObject);
	
}
