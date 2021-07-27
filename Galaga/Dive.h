#pragma once
#include "GameObject.h"

class Dive
{
public:
	Dive(Willem::GameObject* go)
		:m_pGameObject{ go }
	{}
	virtual void Update(float deltaT) = 0;

	bool GetCompleted() const { return m_Completed; };
protected:
	virtual void Enter() {};
	virtual void Exit() {};

	Willem::GameObject* m_pGameObject;
	bool m_Completed = false;
};

