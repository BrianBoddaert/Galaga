#pragma once
#include "Singleton.h"
#include "Structs.h"
#include "GameObject.h"
#include <vector>
#include <map>
#include <memory>
class Dive;

enum class IntroDiveFormation
{
	None,
	ButterFliesAndBeesFromUpToBothSides,
	ButterfliesAndBossesFromLeftToMiddle,
	ButterfliesFromRightToMiddle,
	BeesFromUpToLeft,
	BeesFromUpToRight
};

enum class BombDiveStage
{
	None,
	ButterflyAndBee,
	BossAndTwoButterflies,
	BossTractorBeam
};

enum class EnemyType
{
	Bee,
	Butterfly,
	Boss,
	CapturedPlayer
};

class EnemyManager : public Willem::Singleton<EnemyManager>
	// Also serves as a formation manager
{
public:
	EnemyManager();

	void Update(float);
	void AdjustBombRunCounter(int adjustment) { m_BombRunCounter += adjustment; }

	Willem::Vector2 GetBeeFormationPosition(const Willem::GameObject*) const;
	Willem::Vector2 GetButterflyFormationPosition(const Willem::GameObject*) const;
	Willem::Vector2 GetBossFormationPosition(const Willem::GameObject*) const;

	std::shared_ptr<Willem::GameObject>  SpawnCapturedPlayer(const Willem::Vector2& pos, std::weak_ptr<Willem::GameObject> boss);

	void Reset();

private:
	friend class Willem::Singleton<EnemyManager>;

	int GetIndexOfGameObject(const Willem::GameObject*) const;

	void SpawnAliens(float deltaT);
	void ResetAlienSpawns();

	template<typename T>
	void SpawnBee(const Willem::Vector2& pos);
	template<typename T>
	void SpawnButterfly(const Willem::Vector2& pos);
	template<typename T>
	void SpawnBoss(const Willem::Vector2& pos);

	void SendAliensOnBombRuns(float deltaT);
	void UpdateEnemiesList();
	bool AreThereAnyBossesAlive() const;


	std::map<std::pair<int,EnemyType>, std::weak_ptr<Willem::GameObject>> m_pEnemies;
	// Enemy, Index

	int m_BeeIndexCounter;
	int m_ButterflyIndexCounter;
	int m_BossIndexCounter;
	int m_CapturedPlayerCounter;

	IntroDiveFormation m_IntroDiveFormation;
	BombDiveStage m_BombDiveStage;
	float m_SpawnEnemyTimer = 0.0f;
	int m_EnemySpawnedCounter = 0;
	const float m_SpawnEnemyInterval;

	std::vector<Willem::Vector2> m_BeeFormationLocations;
	std::vector<Willem::Vector2> m_ButterflyFormationLocations;
	std::vector<Willem::Vector2> m_BossFormationLocations;

	//ButterFliesAndBeesFromUpToBothSides
	const int m_FormationSpawnLimit;

	// Altering between upper and lower sprites
	void AlterBetweenSprites(float deltaT);
	bool AreAllEnemiesInFormation();
	const float m_AlteringBetweenSpritesInterval;
	float m_AlteringBetweenSpritesTimer;
	bool m_UpperSpriteActive;
	bool m_SpawnBoss;

	int m_Level;

	int m_BombRunCounter;

	// Enemies movign From left to Right
	void MoveEnemiesFromLeftAndRight(float deltaT);
	bool m_MovingToTheRight;
	const float m_MoveToSidesSpeed = 20.0f;
};

