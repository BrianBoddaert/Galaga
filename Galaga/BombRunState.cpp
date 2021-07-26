#include "stdafx.h"
#include "BombRunState.h"
#include "GameObject.h"

using namespace Willem;

BombRunState::BombRunState(Willem::GameObject* go) noexcept
	:AlienState{ go }
{}

BombRunState::~BombRunState()
{

}

void BombRunState::Update(float)
{

}
AlienState* BombRunState::GetFollowUpState() const { return nullptr; }

void BombRunState::Enter() {};
void BombRunState::Exit() {};