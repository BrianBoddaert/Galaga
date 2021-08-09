#pragma once
#include "Dive.h"
#include "Structs.h"

class BossDive final :
	public Dive
{
public:
	BossDive(Willem::GameObject* go, bool diveToTheRight);
	~BossDive() override;
	void Update(float deltaT) override;

protected:
	void Enter() override;
	void Exit() override;

	Willem::Vector2 GetClosestPlayerPos() const;

	const bool m_DiveToTheRight;
	bool m_HeadingBack;
};

