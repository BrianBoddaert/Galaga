#pragma once
#include "Dive.h"
#include "Structs.h"

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

	Willem::Vector2 GetClosestPlayerPos() const;

	const bool m_DiveToTheRight;
};

