#pragma once
#include "Singleton.h"
#include "Structs.h"
#include "GameObject.h"
#include <vector>

class Dive;

enum class IntroDiveFormation
{
	None,
	ButterFliesAndBeesFromUpToBothSides,
	ButterfliesAndBossesFromLeftToMiddle
};
class EnemyManager : public Willem::Singleton<EnemyManager>
	// Also serves as a formation manager
{
public:
	EnemyManager();
	void SpawnAliens();
	void Update(float);

	void ClaimSpotInBeeFormation(Willem::GameObject*);
	void ClaimSpotInButterflyFormation(Willem::GameObject*);
	void ClaimSpotInBossFormation(Willem::GameObject*);

	void UnclaimSpotInFormation(const Willem::GameObject*);
	void UnclaimSpotInBeeFormation(const Willem::GameObject*);
	void UnclaimSpotInButterflyFormation(const Willem::GameObject*);
	void UnclaimSpotInBossFormation(const Willem::GameObject*);

	Willem::Vector2 GetBeeFormationPosition(const Willem::GameObject*) const;
	Willem::Vector2 GetButterflyFormationPosition(const Willem::GameObject*) const;
	Willem::Vector2 GetBossFormationPosition(const Willem::GameObject*) const;

private:
	friend class Willem::Singleton<EnemyManager>;

	template<typename T>
	void SpawnBee();
	template<typename T>
	void SpawnButterfly();
		template<typename T>
	void SpawnBoss();

	IntroDiveFormation m_IntroDiveFormation;
	float m_SpawnEnemyTimer = 0.0f;
	int m_EnemySpawnedCounter = 0;
	const float m_SpawnEnemyInterval = 0.5f;



	std::vector<Willem::Vector2> m_BeeFormationLocations;
	std::vector<Willem::GameObject*> m_pBeeFormation;
	// Change to pairs? Maybe maps?
	// Order matters, if number 3 dies a new bee will have to go in his place not at the end


	std::vector<Willem::Vector2> m_ButterflyFormationLocations;
	std::vector<Willem::GameObject*> m_pButterflyFormation;

	std::vector<Willem::Vector2> m_BossFormationLocations;
	std::vector<Willem::GameObject*> m_pBossFormation;


	//ButterFliesAndBeesFromUpToBothSides
	const int m_FormationOneSpawnLimit;
};

