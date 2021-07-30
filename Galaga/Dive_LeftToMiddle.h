#pragma once
#include "Dive.h"
class Dive_LeftToMiddle final :
    public Dive
{
public:
	Dive_LeftToMiddle(Willem::GameObject* go);
	~Dive_LeftToMiddle() override;
	void Update(float deltaT) override;
protected:
	void Enter() override;
	void Exit() override;
};

