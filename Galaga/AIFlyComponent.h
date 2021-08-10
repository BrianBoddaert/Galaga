#pragma once
#include "Component.h"
#include <SDL.h>
#include "AlienState.h"
#include "RenderComponent.h"

#include <memory>

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
	AlienState* GetState() const { return m_pState; };

	int GetUpperSrcRectYPos() const { return m_UpperSrcRectYPos; }
	void SetUpperSrcRectYPos(int upperSrcRectYPos) { m_UpperSrcRectYPos = upperSrcRectYPos; }

	void SetCapturedPlayer(std::weak_ptr<Willem::GameObject> go) { m_pCapturedPlayer = go; m_CapturedPlayerActive = true; }
	bool HasCapturedPlayer() const { return m_CapturedPlayerActive; }
	std::weak_ptr<Willem::GameObject> GetCapturedPlayer() const {return m_pCapturedPlayer;}
	template<typename T>
	bool CheckIfStateEqualsTemplate()
	{
		return dynamic_cast<T*>(m_pState);
	}

	void DisableCapturedPlayerActive() { m_CapturedPlayerActive = false; }

private:
	void AdjustSpritesToFitDirection();

	AlienState* m_pState = nullptr;
	Willem::RenderComponent* m_pRenderComponent;
	
	// Only ever used for bosses
	std::weak_ptr<Willem::GameObject> m_pCapturedPlayer;
	bool m_CapturedPlayerActive;

	const float m_IntroDiveSpeed;
	const float m_BombRunSpeed;
	const float m_RotationSpeed;
	float m_RotationRadians;
	int m_UpperSrcRectYPos;

};

