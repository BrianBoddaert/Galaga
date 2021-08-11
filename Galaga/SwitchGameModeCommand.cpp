#include "stdafx.h"
#include "SwitchGameModeCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "EnemyManager.h"
#include "Minigin.h"
#include "ExplosionManager.h"

using namespace Willem;

void SwitchGameModeCommand::Execute(const int&)
{
	EnemyManager::GetInstance().Reset();
	GameMode gameMode =  SceneManager::GetInstance().GetCurrentScene()->GetGameMode();
	if (gameMode == GameMode::Versus)
		gameMode = GameMode::SinglePlayer;
	else
		gameMode = GameMode(int(gameMode) + 1);

	SceneManager::GetInstance().RemoveCurrentScene();
	Minigin::GetInstance().LoadSceneByGameMode(gameMode);
	ExplosionManager::GetInstance().Clear();
}

void SwitchGameModeCommand::Release(const int&)
{
}
void SwitchGameModeCommand::Undo()
{

}


