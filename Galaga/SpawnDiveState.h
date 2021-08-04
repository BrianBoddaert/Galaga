#pragma once
#include "AlienState.h"
#include "Structs.h"
#include "DiveSubState.h"


class GameObject;

class Dive;

class SpawnDiveState final :
    public AlienState
{
public:
	SpawnDiveState(Willem::GameObject* go, Dive* dive) noexcept;
	~SpawnDiveState();
	SpawnDiveState(const SpawnDiveState& other) = delete;
	SpawnDiveState(SpawnDiveState&& other) = delete;
	SpawnDiveState& operator=(const SpawnDiveState& other) = delete;
	SpawnDiveState& operator=(SpawnDiveState&& other) = delete;

	void Update(float deltaT) override;
	AlienState* GetFollowUpState() const override;

protected:
	void Enter() override;
	void Exit() override;

	Dive* m_pDive;

};

