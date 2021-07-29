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
#include "SpawnDiveState.h"
#include "Dive_UpToLeft.h"
#include "Dive_UpToRight.h"

using namespace Willem;

EnemyManager::EnemyManager()
	:m_IntroDiveFormation{IntroDiveFormation::None}
	, m_FormationOneSpawnLimit{8}
{
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Bees.txt", m_BeeFormationLocations);
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Butterflies.txt", m_ButterflyFormationLocations);
}

void EnemyManager::Update(float deltaT)
{
	switch (m_IntroDiveFormation)
	{
	case IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides:
		{
			if (m_EnemySpawnedCounter >= m_FormationOneSpawnLimit)
			{
				m_IntroDiveFormation = IntroDiveFormation::None;
				m_EnemySpawnedCounter = 0;
			}

			m_SpawnEnemyTimer += deltaT;

			if (m_SpawnEnemyTimer >= m_SpawnEnemyInterval)
			{
				m_SpawnEnemyTimer = 0.0f;
				SpawnBee<Dive_UpToLeft>();
				SpawnButterfly<Dive_UpToRight>();
				m_EnemySpawnedCounter++;
				m_EnemySpawnedCounter++;
			}
		}
		break;
	case IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides:
	{
		if (m_EnemySpawnedCounter >= m_FormationOneSpawnLimit)
		{
			m_IntroDiveFormation = IntroDiveFormation::None;
			m_EnemySpawnedCounter = 0;
		}

		m_SpawnEnemyTimer += deltaT;

		if (m_SpawnEnemyTimer >= m_SpawnEnemyInterval)
		{
			m_SpawnEnemyTimer = 0.0f;
			SpawnBee<Dive_UpToLeft>();
			SpawnButterfly<Dive_UpToRight>();
			m_EnemySpawnedCounter++;
			m_EnemySpawnedCounter++;
		}
	}
	break;

	case IntroDiveFormation::None:
		{

		}
		break;
	
	}


}

void EnemyManager::SpawnAliens()
{
	m_IntroDiveFormation = IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides;
}

template <typename T>
void EnemyManager::SpawnBee()
{
	auto bee = std::make_shared<Willem::GameObject>("Bee");

	const SDL_Rect srcRect = { 1,19,16,16 };
	const SDL_Rect halfSize = { srcRect.x/2,srcRect.y/2,srcRect.w/2,srcRect.h/2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w/2.0f - halfSize.w,0,0 };

	bee->AddComponent(new RenderComponent(srcRect));
	bee->SetTexture("Galaga2.png");
	bee->AddComponent(new TransformComponent(spawnPos, 2.0f));
	bee->AddComponent(new HealthComponent(1, false));
	bee->AddComponent(new AIFlyComponent(bee.get(),new SpawnDiveState(bee.get(),new T(bee.get()) )));
	bee->AddTag("Bee");
	bee->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(bee);
	SceneManager::GetInstance().GetCurrentScene()->Add(bee);
}
template <typename T>
void EnemyManager::SpawnButterfly()
{
	auto butterfly = std::make_shared<Willem::GameObject>("Butterfly");

	const SDL_Rect srcRect = { 1,55,16,16 };
	const SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w / 2.0f - halfSize.w,0,0 };

	butterfly->AddComponent(new RenderComponent(srcRect));
	butterfly->SetTexture("Galaga2.png");
	butterfly->AddComponent(new TransformComponent(spawnPos, 2.0f));
	butterfly->AddComponent(new HealthComponent(1, false));
	butterfly->AddComponent(new AIFlyComponent(butterfly.get(), new SpawnDiveState(butterfly.get(), new T(butterfly.get()))));
	butterfly->AddTag("Butterfly");
	butterfly->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(butterfly);
	SceneManager::GetInstance().GetCurrentScene()->Add(butterfly);
}

template <typename T>
void EnemyManager::SpawnBoss(){}

void EnemyManager::ClaimSpotInBeeFormation(Willem::GameObject* go)
{
	if (m_pBeeFormation.size() < m_BeeFormationLocations.size())
		m_pBeeFormation.push_back(go);
}
void EnemyManager::ClaimSpotInButterflyFormation(Willem::GameObject* go)
{
	if (m_pButterflyFormation.size() < m_ButterflyFormationLocations.size())
		m_pButterflyFormation.push_back(go);
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

Vector2 EnemyManager::GetButterflyFormationPosition(const Willem::GameObject* go) const
{
	int index = -1;

	for (int i = 0; i < m_pButterflyFormation.size(); i++)
	{
		if (m_pButterflyFormation[i] == go)
		{
			index = i;
		}
	}


	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for butterfly to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_ButterflyFormationLocations[index];
}

Vector2 EnemyManager::GetBossFormationPosition(const Willem::GameObject*) const
{
	return Vector2{ 0,0 };
}