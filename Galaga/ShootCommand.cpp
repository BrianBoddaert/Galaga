#include "stdafx.h"
#include "ShootCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "ShootComponent.h"

void ShootCommand::Execute(const int&)
{
	int playerIndex = m_ControllerIndex;

	if (m_ControllerIndex == -1)
		playerIndex = 0;

	auto player = Willem::SceneManager::GetInstance().GetCurrentScene()->GetPlayer(playerIndex);
	if (!player)
		return;

	ShootComponent* shootComp = player->GetComponent<ShootComponent>();
	if (!shootComp)
		return;

	Willem::Vector3 dir = Willem::Vector3(0, -1, 0);
	shootComp->Fire(dir);
}

void ShootCommand::Release(const int&)
{
}
void ShootCommand::Undo()
{

}


