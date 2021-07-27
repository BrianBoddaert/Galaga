#pragma once
#include "Dive.h"

class DiveSubState;

class Dive_BeesFromUpToRight final:
    public Dive
{
public:
	Dive_BeesFromUpToRight(Willem::GameObject* go);
	~Dive_BeesFromUpToRight();
	void Update(float deltaT) override;
protected:
	void Enter()override;
	void Exit() override;

	DiveSubState* m_pState;
	int m_DiveProcess;
};

