#pragma once
#include "AlienState.h"
#include "Structs.h"
#include "DiveSubState.h"


class GameObject;

class Dive;

class BombRunState final :
	public AlienState
{
public:
	BombRunState(Willem::GameObject* go, Dive* dive) noexcept;
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

	Dive* m_pDive;

};

