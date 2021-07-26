#pragma once
#include <iostream>
#include "GameObject.h"

class AlienState
{
public:
	AlienState(Willem::GameObject* go) noexcept
		:m_pGameObject{go}
		, m_StateFinished{ false }
		, m_RotationRadians{ 0 }
	{}
	virtual ~AlienState() {}
	AlienState(const AlienState& other) = delete;
	AlienState(AlienState&& other) = delete;
	AlienState& operator=(const AlienState& other) = delete;
	AlienState& operator=(AlienState&& other) = delete;

	virtual void Update(float) {};

	bool GetStateFinished() const { return m_StateFinished; }
	virtual AlienState* GetFollowUpState() const = 0;
protected:
	virtual void Enter() {};
	virtual void Exit() {};

	Willem::GameObject* m_pGameObject;
	bool m_StateFinished;
	float m_RotationRadians;

	void AdjustSpritesToFitDirection();

};

