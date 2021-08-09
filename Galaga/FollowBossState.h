#pragma once
#include "AlienState.h"

class FollowBossState final :
	public AlienState
{
public:
	FollowBossState(Willem::GameObject* go, std::weak_ptr<Willem::GameObject> boss) noexcept;
	~FollowBossState();
	FollowBossState(const FollowBossState& other) = delete;
	FollowBossState(FollowBossState&& other) = delete;
	FollowBossState& operator=(const FollowBossState& other) = delete;
	FollowBossState& operator=(FollowBossState&& other) = delete;

	void Update(float deltaT) override;
	AlienState* GetFollowUpState() const override;
	void SetBoss(std::weak_ptr<Willem::GameObject>  boss) { m_pBoss = boss; }

protected:
	void Enter() override;
	void Exit() override;

	bool m_BossAlive;
	std::weak_ptr<Willem::GameObject> m_pBoss;
};

