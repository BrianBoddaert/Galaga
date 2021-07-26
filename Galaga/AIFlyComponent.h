#pragma once
#include "Component.h"
#include <SDL.h>
#include "AlienState.h"

class GameObject;

class AIFlyComponent : public Willem::Component
{
public:
	AIFlyComponent(Willem::GameObject* gameObject);
	~AIFlyComponent();
	void Update(float) override;

private:

	AlienState* m_pState = nullptr;
};

