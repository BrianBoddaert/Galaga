#include "stdafx.h"
#include "EnemyManager.h"
#include <memory>
#include <SDL.h>
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "HealthComponent.h"
#include "CollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"
#include "AIFlyComponent.h"
#include "TxtParser.h"

using namespace Willem;

EnemyManager::EnemyManager()
{
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Bees.txt", m_BeeFormationLocations);
}

void EnemyManager::Update(float deltaT)
{
	if (m_SpawningEnemies)
	{
		if (m_pBeeFormation.size() < m_BeeFormationLocations.size())
		{
			m_SpawnEnemyTimer += deltaT;

			if (m_SpawnEnemyTimer >= m_SpawnEnemyInterval)
			{
				m_SpawnEnemyTimer = 0.0f;
				SpawnBee();
			}
		}
	}

}

void EnemyManager::SpawnAliens()
{
	m_SpawningEnemies = true;
}

void EnemyManager::SpawnBee()
{
	auto bee = std::make_shared<Willem::GameObject>("Bee");

	const SDL_Rect srcRect = { 0,19,16,16 };
	const SDL_Rect halfSize = { srcRect.x/2,srcRect.y/2,srcRect.w/2,srcRect.h/2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w/2.0f - halfSize.w,0,0 };

	bee->AddComponent(new RenderComponent(srcRect));
	bee->SetTexture("Galaga2.png");
	bee->AddComponent(new TransformComponent(spawnPos, 2.0f));
	bee->AddComponent(new HealthComponent(1, false));
	bee->AddComponent(new AIFlyComponent(bee.get()));
	bee->AddTag("Bee");
	bee->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(bee);
	SceneManager::GetInstance().GetCurrentScene()->Add(bee);
}
void EnemyManager::SpawnButterfly(){}
void EnemyManager::SpawnBoss(){}

void EnemyManager::ClaimSpotInBeeFormation(Willem::GameObject* go)
{
	if (m_pBeeFormation.size() < m_BeeFormationLocations.size())
		m_pBeeFormation.push_back(go);
}
void EnemyManager::ClaimSpotInButterflyFormation(Willem::GameObject*)
{

}
void EnemyManager::ClaimSpotInBossFormation(Willem::GameObject*)
{

}

void EnemyManager::UnclaimSpotInFormation(const Willem::GameObject* go)
{
	UnclaimSpotInBeeFormation(go);
	UnclaimSpotInButterflyFormation(go);
	UnclaimSpotInBossFormation(go);
}
void EnemyManager::UnclaimSpotInBeeFormation(const Willem::GameObject* go)
{
	for (int i = 0; i < m_pBeeFormation.size(); i++)
	{
		if (m_pBeeFormation[i] == go)
		{
			m_pBeeFormation[i] = nullptr;
			return;
		}
	}
}
void EnemyManager::UnclaimSpotInButterflyFormation(const Willem::GameObject* go)
{
	for (int i = 0; i < m_pButterflyFormation.size(); i++)
	{
		if (m_pButterflyFormation[i] == go)
		{
			m_pButterflyFormation[i] = nullptr;
			return;
		}
	}
}
void EnemyManager::UnclaimSpotInBossFormation(const Willem::GameObject* go)
{
	for (int i = 0; i < m_pBossFormation.size(); i++)
	{
		if (m_pBossFormation[i] == go)
		{
			m_pBossFormation[i] = nullptr;
			return;
		}
	}
}

Vector2 EnemyManager::GetBeeFormationPosition(const Willem::GameObject* go) const
{
	int index = -1;

	for (int i = 0; i < m_pBeeFormation.size(); i++)
	{
		if (m_pBeeFormation[i] == go)
		{
			index = i;
		}
	}

	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for bee to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_BeeFormationLocations[index];
}

Vector2 EnemyManager::GetButterflyFormationPosition(const Willem::GameObject*) const
{
	return Vector2{ 0,0 };
}

Vector2 EnemyManager::GetBossFormationPosition(const Willem::GameObject*) const
{
	return Vector2{ 0,0 };
}