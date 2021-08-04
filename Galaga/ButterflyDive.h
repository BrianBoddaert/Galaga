#pragma once
#include "Dive.h"
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

		const bool m_DiveToTheRight;
};

