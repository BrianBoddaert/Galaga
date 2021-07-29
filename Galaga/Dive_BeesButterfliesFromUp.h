#pragma once
#include "Dive.h"

class DiveSubState;

class Dive_BeesButterfliesFromUp final :
	public Dive
{
public:
	Dive_BeesButterfliesFromUp(Willem::GameObject* go);
	~Dive_BeesButterfliesFromUp();
	void Update(float deltaT) override;
protected:
	void Enter()override;
	void Exit() override;

};

