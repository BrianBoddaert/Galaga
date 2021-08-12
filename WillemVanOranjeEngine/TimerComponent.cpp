#include <iostream>
#include "TimerComponent.h"
#include "GameObject.h"

Willem::TimerComponent::TimerComponent()
	:m_TimerDisabled{ 0 }
	, m_TimerEnabled{ 0 }
{

}

void Willem::TimerComponent::Update(float deltaT)
{

	if (m_TimerDisabled > 0.0f)
	{
		m_TimerDisabled -= deltaT;

		if (m_TimerDisabled <= 0)
			m_pGameObject->SetEnabled(true);

	}

	if (m_TimerEnabled > 0.0f)
	{
		m_TimerEnabled -= deltaT;

		if (m_TimerEnabled <= 0)
			m_pGameObject->SetEnabled(false);

	}


}
void Willem::TimerComponent::SetTimerDisabled(const float& time)
{
	m_pGameObject->SetEnabled(false);
	m_TimerDisabled = time;
}
void Willem::TimerComponent::SetTimerEnabled(const float& time)
{
	m_pGameObject->SetEnabled(true);
	m_TimerEnabled = time;
}