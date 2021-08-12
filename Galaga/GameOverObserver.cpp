#include "stdafx.h"
#include "GameOverObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "GameObject.h"
#include "HealthComponent.h"

#include "Minigin.h"
#include "EnemyManager.h"
#include "InputManager.h"

using namespace Willem;

void GameOverObserver::OnNotify(const Willem::GameObject* actor, EVENT event)
{
	if (event == "PlayerDied")
		Unlock(actor, event);
}

void GameOverObserver::Unlock(const Willem::GameObject*, const EVENT&)
{

	auto scene = SceneManager::GetInstance().GetCurrentScene();
	auto players = scene->GetPlayers();
	Minigin& minigin = Minigin::GetInstance();

	for (size_t i = 0; i < players.size(); i++)
		if (players[i]->GetComponent<HealthComponent>()->GetHealthPoints() > 0)
			return;

	const GameMode gameMode = scene->GetGameMode();

	minigin.ClearScreen();
	InputManager::GetInstance().LockInput();
	minigin.LoadGameOverScreen(gameMode);
	EnemyManager::GetInstance().SetEnabled(false);

}