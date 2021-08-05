#pragma once
#include "Dive.h"
class BeeDive final :
	public Dive
{
public:
	BeeDive(Willem::GameObject* go, bool diveToTheRight);
	~BeeDive() override;
	void Update(float deltaT) override;
protected:
	void Enter() override;
	void Exit() override;

	const bool m_DiveToTheRight;
};

