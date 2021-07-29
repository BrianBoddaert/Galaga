#pragma once
#include "GameObject.h"

class DiveSubState;

class Dive
{
public:
	Dive(Willem::GameObject* go)
		:m_pGameObject{ go }
		, m_DiveProcess{ 0 }
		, m_pState{ nullptr }
	{}
	virtual void Update(float deltaT) = 0;

	bool GetCompleted() const { return m_Completed; };
protected:
	virtual void Enter() {};
	virtual void Exit() {};

	Willem::GameObject* m_pGameObject;
	bool m_Completed = false;
	DiveSubState* m_pState;
	int m_DiveProcess;
};

