#include "stdafx.h"
#include "SwitchGameModeCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "EnemyManager.h"
#include "Minigin.h"

using namespace Willem;

void SwitchGameModeCommand::Execute(const int&)
{

	GameMode gameMode =  SceneManager::GetInstance().GetCurrentScene()->GetGameMode();
	if (gameMode == GameMode::Versus)
		gameMode = GameMode::SinglePlayer;
	else
		gameMode = GameMode(int(gameMode) + 1);

	Minigin::GetInstance().ClearScreen();
	Minigin::GetInstance().LoadSceneByGameMode(gameMode);

}

void SwitchGameModeCommand::Release(const int&)
{
}
void SwitchGameModeCommand::Undo()
{

}


