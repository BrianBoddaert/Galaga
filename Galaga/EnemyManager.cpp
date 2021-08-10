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

#include "BombRunState.h"
#include "ButterflyDive.h"
#include "BeeDive.h"
#include "BossDive.h"
#include "ShootComponent.h"
#include "TractorBeamDive.h"

#include "FollowBossState.h"

using namespace Willem;

EnemyManager::EnemyManager()
	:m_IntroDiveFormation{ IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides }
	, m_FormationSpawnLimit{ 8 }
	, m_AlteringBetweenSpritesInterval{ 1.0f }
	, m_AlteringBetweenSpritesTimer{ 0.0f }
	, m_UpperSpriteActive{ true }
	, m_SpawnBoss{ false }
	, m_BeeIndexCounter{ 0 }
	, m_ButterflyIndexCounter{ 0 }
	, m_BossIndexCounter{ 0 }
	, m_CapturedPlayerCounter{ 0 }
	, m_SpawnEnemyInterval{ 0.1f }
	, m_BombDiveStage{ BombDiveStage::None }
	, m_Level{ 1 }
{
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Bees.txt", m_BeeFormationLocations);
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Butterflies.txt", m_ButterflyFormationLocations);
	TxtParser::GetInstance().Parse("../Data/Formations/Formation1Boss.txt", m_BossFormationLocations);
}

void EnemyManager::Update(float deltaT)
{
	UpdateEnemiesList();
	AlterBetweenSprites(deltaT);
	SpawnAliens(deltaT);
	SendAliensOnBombRuns(deltaT);
}

bool EnemyManager::AreAllEnemiesInFormation()
{
	std::map<std::pair<int, EnemyType>, std::weak_ptr<Willem::GameObject>>::iterator it;

	for (it = m_pEnemies.begin(); it != m_pEnemies.end(); it++)
	{
		std::shared_ptr<Willem::GameObject> enemy = it->second.lock();
		if (!enemy->GetComponent<AIFlyComponent>()->CheckIfStateEqualsTemplate<FormationState>())
			return false;

	}

	return true;
}

void EnemyManager::ResetAlienSpawns()
{
	m_BeeIndexCounter = 0;
	m_ButterflyIndexCounter = 0;
	m_BossIndexCounter = 0;
	m_CapturedPlayerCounter = 0;

	m_IntroDiveFormation = IntroDiveFormation::ButterFliesAndBeesFromUpToBothSides;
	m_BombDiveStage = BombDiveStage::None;
	m_SpawnEnemyTimer = 0.0f;
	m_EnemySpawnedCounter = 0;
	m_SpawnBoss = false;

	if (m_Level < 3)
		m_Level++;

	m_BeeFormationLocations.clear();
	m_ButterflyFormationLocations.clear();
	m_BossFormationLocations.clear();

	std::string path = "../Data/Formations/Formation" + std::to_string(m_Level) + "Bees.txt";
	TxtParser::GetInstance().Parse(path, m_BeeFormationLocations);

	path = "../Data/Formations/Formation" + std::to_string(m_Level) + "Butterflies.txt";
	TxtParser::GetInstance().Parse(path, m_ButterflyFormationLocations);

	path = "../Data/Formations/Formation" + std::to_string(m_Level) + "Boss.txt";
	TxtParser::GetInstance().Parse(path, m_BossFormationLocations);


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

		if (m_EnemySpawnedCounter < m_FormationSpawnLimit)
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

			m_BeeIndexCounter = 0;
			m_ButterflyIndexCounter = 0;
			m_BossIndexCounter = 0;
		}
	}
	break;
	case IntroDiveFormation::None:
	{
		if (m_pEnemies.empty())
			ResetAlienSpawns();
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
	bee->AddComponent(new ShootComponent());
	bee->AddComponent(new AIFlyComponent(bee.get(), new SpawnDiveState(bee.get(), new T(bee.get())), srcRect.y));
	bee->AddTag("Bee");
	bee->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(bee);
	SceneManager::GetInstance().GetCurrentScene()->Add(bee);

	std::weak_ptr<Willem::GameObject> weakPtr = bee;
	m_pEnemies[std::make_pair(m_BeeIndexCounter,EnemyType::Bee)] = weakPtr;
	m_BeeIndexCounter++;
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
	butterfly->AddComponent(new ShootComponent());
	butterfly->AddComponent(new AIFlyComponent(butterfly.get(), new SpawnDiveState(butterfly.get(), new T(butterfly.get())), srcRect.y));
	butterfly->AddTag("Butterfly");
	butterfly->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(butterfly);
	SceneManager::GetInstance().GetCurrentScene()->Add(butterfly);

	std::weak_ptr<Willem::GameObject> weakPtr = butterfly;
	m_pEnemies[std::make_pair(m_ButterflyIndexCounter, EnemyType::Butterfly)] = weakPtr;
	m_ButterflyIndexCounter++;
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
	boss->AddComponent(new ShootComponent());
	boss->AddComponent(new AIFlyComponent(boss.get(), new SpawnDiveState(boss.get(), new T(boss.get())), srcRect.y));
	boss->AddComponent(new HealthComponent(2, false));
	boss->AddTag("Boss");
	boss->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(boss);
	SceneManager::GetInstance().GetCurrentScene()->Add(boss);

	std::weak_ptr<Willem::GameObject> weakPtr = boss;
	m_pEnemies[std::make_pair(m_BossIndexCounter, EnemyType::Boss)] = weakPtr;
	m_BossIndexCounter++;
}

std::shared_ptr<Willem::GameObject> EnemyManager::SpawnCapturedPlayer(const Willem::Vector2& pos, std::weak_ptr<Willem::GameObject> boss)
{
	auto capturedPlayer = std::make_shared<Willem::GameObject>("CapturedPlayer");

	const SDL_Rect srcRect = { 1,163,16,16 };
	const SDL_Rect halfSize = { srcRect.x / 2,srcRect.y / 2,srcRect.w / 2,srcRect.h / 2 };
	const SDL_Surface* surface = Minigin::GetWindowSurface();
	const Vector3 spawnPos = { surface->w / 2.0f - halfSize.w,0,0 };

	capturedPlayer->AddComponent(new RenderComponent(srcRect));
	capturedPlayer->SetTexture("Galaga2.png");
	capturedPlayer->AddComponent(new TransformComponent(Vector3{ pos.x,pos.y,1.0f }, float(GAMESCALE)));
	capturedPlayer->AddComponent(new HealthComponent(1));
	capturedPlayer->AddComponent(new ShootComponent());
	auto aiFlyComp = new AIFlyComponent(capturedPlayer.get(), nullptr, srcRect.y);
	aiFlyComp->SetState(new FollowBossState(capturedPlayer.get(), boss));
	capturedPlayer->AddComponent(aiFlyComp);
	capturedPlayer->AddTag("CapturedPlayer");
	capturedPlayer->AddTag("Alien");

	CollisionManager::GetInstance().AddCollider(capturedPlayer);
	SceneManager::GetInstance().GetCurrentScene()->Add(capturedPlayer);

	std::weak_ptr<Willem::GameObject> weakPtr = capturedPlayer;
	m_pEnemies[std::make_pair(m_CapturedPlayerCounter, EnemyType::CapturedPlayer)] = weakPtr;

	m_CapturedPlayerCounter++;

	return capturedPlayer;
}

void EnemyManager::AlterBetweenSprites(float deltaT)
{
	m_AlteringBetweenSpritesTimer += deltaT;

	if (m_AlteringBetweenSpritesTimer < m_AlteringBetweenSpritesInterval)
		return;

	m_AlteringBetweenSpritesTimer = 0.0f;

	std::map<std::pair<int, EnemyType>, std::weak_ptr<Willem::GameObject>>::iterator it;

	for (it = m_pEnemies.begin(); it != m_pEnemies.end(); it++)
	{
		std::shared_ptr<Willem::GameObject> enemy = it->second.lock();
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

	m_UpperSpriteActive = !m_UpperSpriteActive;

	m_BombDiveStage = BombDiveStage::None;

}

int EnemyManager::GetIndexOfGameObject(const Willem::GameObject* go) const
{
	int index = -1;

	for (auto& pair : m_pEnemies)
	{
		if (pair.second.lock().get() == go)
		{
			index = pair.first.first;
			break;
		}
	}

	return index;
}

Vector2 EnemyManager::GetBeeFormationPosition(const Willem::GameObject* go) const
{
	int index = GetIndexOfGameObject(go);

	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for bee to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_BeeFormationLocations[index];
}
Vector2 EnemyManager::GetButterflyFormationPosition(const Willem::GameObject* go) const
{
	int index = GetIndexOfGameObject(go);

	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for butterfly to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_ButterflyFormationLocations[index];
}
Vector2 EnemyManager::GetBossFormationPosition(const Willem::GameObject* go) const
{
	int index = GetIndexOfGameObject(go);

	if (index == -1) // NO SPOTS LEFT
	{
		std::cout << "No spot left in formation for boss to park in" << std::endl;
		return Vector2(0, 0);
	}

	return m_BossFormationLocations[index];
}


void EnemyManager::UpdateEnemiesList()
{
	std::map<std::pair<int, EnemyType>, std::weak_ptr<Willem::GameObject>>::iterator it;

	for (it = m_pEnemies.begin(); it != m_pEnemies.end();)
	{
		if (it->second.use_count() <= 1)
			m_pEnemies.erase(it++);
		else
			++it;
	}
}
void EnemyManager::SendAliensOnBombRuns(float)
{
	if (m_IntroDiveFormation != IntroDiveFormation::None || m_BombDiveStage != BombDiveStage::None)
		return;

	m_BombDiveStage = BombDiveStage::ButterflyAndBee;

	bool foundBee = false;
	bool foundBF = false;
	bool foundBoss = false;

	for (auto& pair : m_pEnemies)
	{
		Willem::GameObject* go = pair.second.lock().get();
		AIFlyComponent* flyComp = go->GetComponent<AIFlyComponent>();
		TransformComponent* transformComponent = go->GetComponent<TransformComponent>();
		const SDL_Surface* surface = Minigin::GetWindowSurface();

		if (flyComp->CheckIfStateEqualsTemplate<BombRunState>())
			continue;

		if (!foundBee && pair.first.second == EnemyType::Bee)
		{
			foundBee = true;
			//flyComp->SetState(new BombRunState(go, new BeeDive(go, transformComponent->GetPosition().x < (surface->w/2))));

		}
		else if (!foundBF && pair.first.second == EnemyType::Butterfly)
		{
			foundBF = true;
			//flyComp->SetState(new BombRunState(go, new ButterflyDive(go, transformComponent->GetPosition().x < (surface->w / 2))));
		}
		else if (!foundBoss && pair.first.second == EnemyType::Boss)
		{
			foundBoss = true;
			/*flyComp->SetState(new BombRunState(go, new BossDive(go, transformComponent->GetPosition().x < (surface->w / 2))));*/
			flyComp->SetState(new BombRunState(go, new TractorBeamDive(go, transformComponent->GetPosition().x < (surface->w / 2))));
		}
	}
}