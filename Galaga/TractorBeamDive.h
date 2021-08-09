#pragma once
#include "Dive.h"
#include "Structs.h"

class TractorBeamDive final :
	public Dive
{
public:
	TractorBeamDive(Willem::GameObject* go, bool diveToTheRight);
	~TractorBeamDive() override;
	void Update(float deltaT) override;

protected:
	void Enter() override;
	void Exit() override;

	Willem::Vector2 GetClosestPlayerPos() const;

	const bool m_DiveToTheRight;
};

