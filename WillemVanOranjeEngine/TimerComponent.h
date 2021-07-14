#pragma once
#include "Component.h"

namespace Willem
{

	class TextComponent;
	class TimerComponent : public Component
	{
	public:
		TimerComponent();
		void Update(float deltaT) override;
		void SetTimerDisabled(const float&);
		void SetTimerEnabled(const float&);
	private:
		float m_TimerEnabled;
		float m_TimerDisabled;

	};
}