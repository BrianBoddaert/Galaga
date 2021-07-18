#include "stdafx.h"
#include "MoveCommand.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ControlComponent.h"

void MoveCommand::Execute(const int& dir)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;


	auto player = Willem::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex);
	if (!player)
		return;
	const auto& controlComp = player.get()->GetComponent<ControlComponent>();
	controlComp->SetMoveInput(MoveInputDirections(dir), true);
}

void MoveCommand::Release(const int& dir)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;

	auto player = Willem::SceneManager::GetInstance().GetCurrentScene().get()->GetPlayer(playerIndex);
	if (!player)
		return;

	const auto& controlComp = player.get()->GetComponent<ControlComponent>();
	controlComp->SetMoveInput(MoveInputDirections(dir), false);
}
void MoveCommand::Undo()
{

}


