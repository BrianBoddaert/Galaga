#include "stdafx.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
#include "Observer.h"
#include <string>

ScoreComponent::ScoreComponent(const unsigned int& score)
	:m_Score{ score }
{}

void ScoreComponent::SetScore(const unsigned int& score)
{
	m_Score = score;
	m_pGameObject->Notify("SetScore");
}


void ScoreComponent::Update(float)
{
}

const unsigned int& ScoreComponent::GetScore() const
{
	return m_Score;
}

void ScoreComponent::IncreaseScore(const EVENT& e)
{
	if (e == "BeeFormation")
		m_Score += 50;
	else if (e == "BeeDiving")
		m_Score += 100;
	else if (e == "ButterflyFormation")
		m_Score += 80;
	else if (e == "ButterflyDiving")
		m_Score += 160;
	else if (e == "BossFormation")
		m_Score += 150;
	else if (e == "BossDiving")
		m_Score += 400;

	m_pGameObject->Notify(e);
}

