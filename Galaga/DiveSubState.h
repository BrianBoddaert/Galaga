#pragma once
#include "GameObject.h"

class SpawnDiveState;

class DiveSubState
{
public:
	DiveSubState(Willem::GameObject* go) noexcept
		:m_pGameObject{ go }
		, m_SubStateFinished{ false }
	{}
	virtual ~DiveSubState() {}
	DiveSubState(const DiveSubState& other) = delete;
	DiveSubState(DiveSubState&& other) = delete;
	DiveSubState& operator=(const DiveSubState& other) = delete;
	DiveSubState& operator=(DiveSubState&& other) = delete;

	virtual void Update(float) {};
	bool GetStateFinished() const { return m_SubStateFinished; }

protected:
	virtual void Enter() {};
	virtual void Exit() {};

	Willem::GameObject* m_pGameObject;
	bool m_SubStateFinished;
};

