#pragma once
#include "Dive.h"
class Dive_RightToMiddle final :
	public Dive
{
public:
	Dive_RightToMiddle(Willem::GameObject* go);
	~Dive_RightToMiddle() override;
	void Update(float deltaT) override;
protected:
	void Enter() override;
	void Exit() override;
};

