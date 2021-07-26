#pragma once
#include <Component.h>
class HealthComponent :
    public Willem::Component
{
public:
	HealthComponent(int health = 1, bool removeWhenOutOfScreen = false);
	void Update(float deltaT) override;
	const int GetHealthPoints() const;
	void Hit(int damage = 1);
	void Heal(int amount = 1);
private:
	void Die();

	int m_HealthPoints;
	const int m_MaxHealthPoints;
	const bool m_RemoveWhenOutOfScreen;
};

