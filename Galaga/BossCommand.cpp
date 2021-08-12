#include "stdafx.h"
#include "BossCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ControlComponent.h"
#include "ShootComponent.h"

#include "EnemyManager.h"
#include "TransformComponent.h"
#include "AIFlyComponent.h"
#include "BombRunState.h"
#include "TractorBeamDive.h"
#include "Minigin.h"
#include "FormationState.h"
#include "ButterflyDive.h"

using namespace Willem;

void BossCommand::Execute(const int& dir)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;

	if (playerIndex != 1)
		return;

	auto& enemyManager = EnemyManager::GetInstance();

	if (!enemyManager.IsTheIntroDiveCompleted())
		return;

	// Find alien
	bool foundBoss = false;
	auto boss = enemyManager.GetFirstBoss(foundBoss);

	if (!foundBoss)
		return;

	if (dir == (int)BossMoves::Shoot)
	{
	ShootComponent* shootComp = boss.lock()->GetComponent<ShootComponent>();
	if (!shootComp)
		return;

	if (!enemyManager.CanVersusModeBossFire())
		return;

	enemyManager.SetVersusModeShootTimer(0.0f);

	Vector2 pos = boss.lock()->GetComponent<TransformComponent>()->GetPosition();
	Vector2 playerPos = SceneManager::GetInstance().GetCurrentScene()->GetPlayer(0)->GetComponent<TransformComponent>()->GetPosition();

	shootComp->DoubleFire((playerPos - pos).Normalize());

	}
	else if (dir == (int)BossMoves::Dive)
	{
		auto go = boss.lock().get();
		auto flyComp = go->GetComponent<AIFlyComponent>();
		Vector2 pos = go->GetComponent<TransformComponent>()->GetPosition();
		auto surface = Minigin::GetWindowSurface();

		if (flyComp->CheckIfStateEqualsTemplate<FormationState>())
			flyComp->SetState(new BombRunState(go, new ButterflyDive(go, pos.x < (surface->w / 2))));
	}
	else if (dir == (int)BossMoves::TractorBeam)
	{
		auto go = boss.lock().get();
		auto flyComp = go->GetComponent<AIFlyComponent>();
		Vector2 pos = go->GetComponent<TransformComponent>()->GetPosition();
		auto surface = Minigin::GetWindowSurface();

		if (flyComp->CheckIfStateEqualsTemplate<FormationState>())
			flyComp->SetState(new BombRunState(go, new TractorBeamDive(go, pos.x < (surface->w / 2))));
	}



}



void BossCommand::Release(const int&)
{

}
void BossCommand::Undo()
{

}
