#pragma once
#include "Dive.h"

class Dive_UpToLeft final :
	public Dive
{
public:
	Dive_UpToLeft(Willem::GameObject* go);
	~Dive_UpToLeft() override;
	void Update(float deltaT) override;
protected:
	void Enter() override;
	void Exit() override;
};