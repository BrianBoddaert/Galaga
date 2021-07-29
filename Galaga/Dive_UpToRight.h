#pragma once
#include "Dive.h"

class Dive_UpToRight final:
    public Dive
{
public:
	Dive_UpToRight(Willem::GameObject* go);
	~Dive_UpToRight();
	void Update(float deltaT) override;
protected:
	void Enter() override;
	void Exit() override;
};

