#include "stdafx.h"
#include "FormationState.h"
#include "BombRunState.h"
#include <SDL.h>

FormationState::FormationState(Willem::GameObject* go) noexcept
	:AlienState{ go }
{
	Enter();
}

FormationState::~FormationState()
{
	Exit();
}


void FormationState::Update(float deltaT)
{

}

AlienState* FormationState::GetFollowUpState() const
{
	return new BombRunState(m_pGameObject);
}

void FormationState::Enter()
{
	m_RotationRadians = float(M_PI);

	AdjustSpritesToFitDirection();
}

void FormationState::Exit() {}