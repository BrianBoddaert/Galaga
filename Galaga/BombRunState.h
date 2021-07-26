#pragma once
#include "AlienState.h"

class GameObject;

class BombRunState final :
    public AlienState
{
public:
	BombRunState(Willem::GameObject* go) noexcept;
	~BombRunState();
	BombRunState(const BombRunState& other) = delete;
	BombRunState(BombRunState&& other) = delete;
	BombRunState& operator=(const BombRunState& other) = delete;
	BombRunState& operator=(BombRunState&& other) = delete;

	void Update(float deltaT) override;
	AlienState* GetFollowUpState() const override;

protected:
	void Enter() override;
	void Exit() override;
};

