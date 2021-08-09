#pragma once
#include "Component.h"
#include <SDL.h>
#include "AlienState.h"
#include "RenderComponent.h"

class SpawnDiveState;
class GameObject;


class AIFlyComponent : public Willem::Component
{
public:
	AIFlyComponent(Willem::GameObject* gameObject, SpawnDiveState* state, int srcRectYPos);
	~AIFlyComponent();
	void Update(float) override;

	float GetIntroDiveSpeed() const { return m_IntroDiveSpeed; };
	float GetBombRunSpeed() const { return m_BombRunSpeed; };
	float GetRotationSpeed() const { return m_RotationSpeed; };

	void SetRotationRadians(float radians) { m_RotationRadians = radians;};
	float GetRotationRadians() const { return m_RotationRadians; };

	void SetState(AlienState*);

	int GetUpperSrcRectYPos() const { return m_UpperSrcRectYPos; }
	void SetUpperSrcRectYPos(int upperSrcRectYPos) { m_UpperSrcRectYPos = upperSrcRectYPos; }

	template<typename T>
	bool CheckIfStateEqualsTemplate()
	{
		return dynamic_cast<T*>(m_pState);
	}

private:
	void AdjustSpritesToFitDirection();

	AlienState* m_pState = nullptr;
	Willem::RenderComponent* m_pRenderComponent;

	const float m_IntroDiveSpeed;
	const float m_BombRunSpeed;
	const float m_RotationSpeed;
	float m_RotationRadians;
	int m_UpperSrcRectYPos;

};

