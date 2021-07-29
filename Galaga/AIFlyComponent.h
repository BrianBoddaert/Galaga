#pragma once
#include "Component.h"
#include <SDL.h>
#include "AlienState.h"

class SpawnDiveState;

class GameObject;

class AIFlyComponent : public Willem::Component
{
public:
	AIFlyComponent(Willem::GameObject* gameObject);
	AIFlyComponent(Willem::GameObject* gameObject, SpawnDiveState* state);
	~AIFlyComponent();
	void Update(float) override;

	float GetSpeed() const { return m_Speed; };
	float GetRotationSpeed() const { return m_RotationSpeed; };

	void AdjustSpritesToFitDirection();

	void SetRotationRadians(float radians) { m_RotationRadians = radians;};
	float GetRotationRadians() const { return m_RotationRadians; };

	void SetSpawnDiveState(SpawnDiveState*);
private:

	AlienState* m_pState = nullptr;

	const float m_Speed;
	const float m_RotationSpeed;
	float m_RotationRadians;
};

