#pragma once
#include "Component.h"
#include "GameObject.h"

class TextComponent;
class ScoreComponent final : public Willem::Component
{
public:
	ScoreComponent(const unsigned int& score);
	void Update(float deltaT) override;
	const unsigned int& GetScore() const;
	void SetScore(const unsigned int&);
	void IncreaseScore(const EVENT& e);
private:
	unsigned int m_Score;

};
