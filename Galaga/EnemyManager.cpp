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
#include "Dive_LeftToMiddle.h"
#include "Dive_RightToMiddle.h"
#include "FormationState.h"

using namespace Willem;

EnemyManager::EnemyManager()
	:m_IntroDiveFormation{IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides }
	, m_FormationSpawnLimit{8}
	, m_AlteringBetweenSpritesInterval{1.0f}
	, m_AlteringBetweenSpritesTimer{0.0f}
	, m_UpperSpriteActive{true}
	, m_SpawnBoss{ false }
{
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Bees.txt", m_BeeFormationLocations);
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Butterflies.txt", m_ButterflyFormationLocations);
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Boss.txt", m_BossFormationLocations);
}

void EnemyManager::Update(float deltaT)
{
	AlterBetweenSprites(deltaT);
	SpawnAliens(deltaT);

}

bool EnemyManager::AreAllEnemiesInFormation()
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i].use_count() <= 1)
			m_pEnemies.erase(m_pEnemies.begin() + i);
		else
		{
			std::shared_ptr<Willem::GameObject> enemy = m_pEnemies[i].lock();
			if (!enemy->GetComponent<AIFlyComponent>()->CheckIfStateEqualsTemplate<FormationState>())
				return false;
		}

	}

	return true;
}

void EnemyManager::SpawnAliens(float deltaT)
{
	m_SpawnEnemyTimer += deltaT;

	if (m_SpawnEnemyTimer < m_SpawnEnemyInterval)
		return;

	switch (m_IntroDiveFormation)
	{
	case IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides:
	{

		if ( m_EnemySpawnedCounter < m_FormationSpawnLimit)
		{
			const int halfWidthOfSrcRect = 8;
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector3 spawnPos = { surface->w / 2.0f - halfWidthOfSrcRect,0,0 };

			m_SpawnEnemyTimer = 0.0f;
			SpawnBee<Dive_UpToLeft>(spawnPos);
			SpawnButterfly<Dive_UpToRight>(spawnPos);
			m_EnemySpawnedCounter++;
			m_EnemySpawnedCounter++;
		}
		else if (AreAllEnemiesInFormation())
		{
			m_IntroDiveFormation = IntroDiveFormation::ButterfliesAndBossesFromLeftToMiddle;
			m_EnemySpawnedCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::ButterfliesAndBossesFromLeftToMiddle:
	{

		if (m_EnemySpawnedCounter < m_FormationSpawnLimit)
		{
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector3 spawnPos = { 0,surface->h - 50.0f,0 };

			m_SpawnEnemyTimer = 0.0f;
				if (m_SpawnBoss)
					SpawnBoss<Dive_LeftToMiddle>(spawnPos);
				else
					SpawnButterfly<Dive_LeftToMiddle>(spawnPos);
			m_EnemySpawnedCounter++;
			m_SpawnBoss = !m_SpawnBoss;

		}
		else if (AreAllEnemiesInFormation())
		{
			m_IntroDiveFormation = IntroDiveFormation::ButterfliesFromRightToMiddle;
			m_EnemySpawnedCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::ButterfliesFromRightToMiddle:
	{
		if (m_EnemySpawnedCounter < m_FormationSpawnLimit)
		{
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector3 spawnPos = { float(surface->w),surface->h - 50.0f,0 };

			m_SpawnEnemyTimer = 0.0f;
			SpawnButterfly<Dive_RightToMiddle>(spawnPos);
			m_EnemySpawnedCounter++;
		}
		else if (AreAllEnemiesInFormation())
		{
			m_IntroDiveFormation = IntroDiveFormation::BeesFromUpToLeft;
			m_EnemySpawnedCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::BeesFromUpToLeft:
	{
		if (m_EnemySpawnedCounter < m_FormationSpawnLimit)
		{
			const int halfWidthOfSrcRect = 8;
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector3 spawnPos = { surface->w / 2.0f - halfWidthOfSrcRect,0,0 };

			m_SpawnEnemyTimer = 0.0f;
			SpawnBee<Dive_UpToLeft>(spawnPos);
			m_EnemySpawnedCounter++;
		}
		else if (AreAllEnemiesInFormation())
		{
			m_IntroDiveFormation = IntroDiveFormation::BeesFromUpToRight;
			m_EnemySpawnedCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::BeesFromUpToRight:
	{
		if (m_EnemySpawnedCounter < m_FormationSpawnLimit)
		{
			const int halfWidthOfSrcRect = 8;
			const SDL_Surface* surface = Minigin::GetWindowSurface();
			const Vector3 spawnPos = { surface->w / 2.0f - halfWidthOfSrcRect,0,0 };

			m_SpawnEnemyTimer = 0.0f;
			SpawnBee<Dive_UpToRight>(spawnPos);
			m_EnemySpawnedCounter++;
		}
		else if (AreAllEnemiesInFormation())
		{
			m_IntroDiveFormation = IntroDiveFormation::None;
			m_EnemySpawnedCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::None:
	{

	}
	break;

	}
}

template <typename T>
void EnemyManager::SpawnBee(const Willem::Vector2& pos)
{
	auto bee = std::make_shared<Willem::GameObject>("Bee");

	const SDL_Rect srcRect = { 1,19,16,16 };

	bee->AddComponent(new RenderComponent(srcRect));
	bee->SetTexture("Galaga2.png");
	bee->AddComponent(new TransformComponent(Vector3{ pos.x,pos.y,1.0f }, float(GAMESCALE)));
	bee->AddComponent(new HealthComponent(1, false));
	bee->AddComponent(new AIFlyComponent(bee.get(),new SpawnDiveState(bee.get(),new T(bee.get())), srcRect.y));
	bee->AddTag("Bee");
	bee->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(bee);
	SceneManager::GetInstance().GetCurrentScene()->Add(bee);
	m_pEnemies.push_back(bee);
}

template <typename T>
void EnemyManager::SpawnButterfly(const Willem::Vector2& pos)
{
	auto butterfly = std::make_shared<Willem::GameObject>("Butterfly");

	const SDL_Rect srcRect = { 1,55,16,16 };
	const SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w / 2.0f - halfSize.w,0,0 };

	butterfly->AddComponent(new RenderComponent(srcRect));
	butterfly->SetTexture("Galaga2.png");
	butterfly->AddComponent(new TransformComponent(Vector3{ pos.x,pos.y,1.0f }, float(GAMESCALE)));
	butterfly->AddComponent(new HealthComponent(1, false));
	butterfly->AddComponent(new AIFlyComponent(butterfly.get(), new SpawnDiveState(butterfly.get(), new T(butterfly.get())), srcRect.y));
	butterfly->AddTag("Butterfly");
	butterfly->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(butterfly);
	SceneManager::GetInstance().GetCurrentScene()->Add(butterfly);
	m_pEnemies.push_back(butterfly);
}

template <typename T>
void EnemyManager::SpawnBoss(const Willem::Vector2& pos) 
{
	auto boss = std::make_shared<Willem::GameObject>("Boss");

	const SDL_Rect srcRect = { 1,91,16,16 };
	const SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w / 2.0f - halfSize.w,0,0 };

	boss->AddComponent(new RenderComponent(srcRect));
	boss->SetTexture("Galaga2.png");
	boss->AddComponent(new TransformComponent(Vector3{ pos.x,pos.y,1.0f }, float(GAMESCALE)));
	boss->AddComponent(new HealthComponent(2, false));
	boss->AddComponent(new AIFlyComponent(boss.get(), new SpawnDiveState(boss.get(), new T(boss.get())), srcRect.y));
	boss->AddTag("Boss");
	boss->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(boss);
	SceneManager::GetInstance().GetCurrentScene()->Add(boss);
	m_pEnemies.push_back(boss);
}


void EnemyManager::AlterBetweenSprites(float deltaT)
{
	m_AlteringBetweenSpritesTimer += deltaT;

	if (m_AlteringBetweenSpritesTimer < m_AlteringBetweenSpritesInterval)
		return;

	m_AlteringBetweenSpritesTimer = 0.0f;

	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i].use_count() <= 1)
			m_pEnemies.erase(m_pEnemies.begin() + i);
		else
		{
			std::shared_ptr<Willem::GameObject> enemy = m_pEnemies[i].lock(); 
			const int upperSrcRectYPos = enemy->GetComponent<AIFlyComponent>()->GetUpperSrcRectYPos();
			RenderComponent* renderComp = enemy->GetComponent<RenderComponent>();
			SDL_Rect srcRect = renderComp->GetSrcRect();
			const int offsetHeight = 2;

			if (m_UpperSpriteActive)
				srcRect.y = upperSrcRectYPos + srcRect.h + offsetHeight;
			else
				srcRect.y = upperSrcRectYPos;

			renderComp->SetSrcRect(srcRect);

		}
			
	}

	m_UpperSpriteActive = !m_UpperSpriteActive;
}

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
void EnemyManager::ClaimSpotInBossFormation(Willem::GameObject* go)
{
	if (m_pBossFormation.size() < m_BossFormationLocations.size())
		m_pBossFormation.push_back(go);
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
Vector2 EnemyManager::GetBossFormationPosition(const Willem::GameObject* go) const
{
	int index = -1;

	for (int i = 0; i < m_pBossFormation.size(); i++)
	{
		if (m_pBossFormation[i] == go)
		{
			index = i;
		}
	}


	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for boss to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_BossFormationLocations[index];
}