#include "stdafx.h"
#include "ScoreObserver.h"
#include "SceneManager.h"
#include "Scene.h"

#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

using namespace Willem;

void ScoreObserver::OnNotify(const Willem::GameObject* actor, EVENT event)
{
	if (event == "BeeFormation" || event == "BeeDiving" || event == "ButterflyFormation" || event == "ButterflyDiving" || event == "BossFormation" || event == "BossDiving")
		Unlock(actor, event);
}

void ScoreObserver::Unlock(const Willem::GameObject* actor, const EVENT&)
{

	std::shared_ptr < Willem::GameObject > pointsDisplay = nullptr;

	if (actor->HasTag("Player1"))
		pointsDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreHUD");
	else if (actor->HasTag("Player2"))
		pointsDisplay = SceneManager::GetInstance().GetCurrentScene().get()->GetObjectByName("ScoreHUD2");

	auto textComp = pointsDisplay->GetComponent<Willem::TextComponent>();
	textComp->SetText(std::to_string(actor->GetComponent<ScoreComponent>()->GetScore()));


}