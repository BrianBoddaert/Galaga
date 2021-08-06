#pragma once
#include "Dive.h"
#include "Structs.h"

class ButterflyDive final:
    public Dive
{
	public:
		ButterflyDive(Willem::GameObject* go,bool diveToTheRight);
		~ButterflyDive() override;
		void Update(float deltaT) override;
	protected:
		void Enter() override;
		void Exit() override;

		Willem::Vector2 GetClosestPlayerPos() const;

		const bool m_DiveToTheRight;
		bool m_HeadingBack;
		bool m_FirstCycle;
};

