#pragma once
#include "Singleton.h"
#include "Structs.h"
#include "GameObject.h"

class EnemyManager : public Willem::Singleton<EnemyManager>
	// Also serves as a formation manager
{
public:
	void SpawnAliens();
	void Update(float) {};

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

	void SpawnBee();
	void SpawnButterfly();
	void SpawnBoss();
	


	static const int m_BeeFormationSize = 10;
	static const int m_BeeFormationRowCount = 2;
	static const int m_ButterflyFormationSize = 8;
	static const int m_ButterflyFormationRowCount = 2;
	static const int m_BossFormationSize = 4;
	static const int m_BossFormationRowCount = 1;

	Willem::GameObject* m_pBeeFormation[m_BeeFormationSize * m_BeeFormationRowCount];
	Willem::GameObject* m_pButterflyFormation[m_ButterflyFormationSize * m_ButterflyFormationRowCount];
	Willem::GameObject* m_pBossFormation[m_BossFormationSize * m_BossFormationRowCount];
};

