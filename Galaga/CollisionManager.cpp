#include "stdafx.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "ControlComponent.h"

#include "RenderComponent.h"
#include "ShootComponent.h"

#include "SceneManager.h"
#include "Scene.h"
#include "ScoreComponent.h"
#include "AIFlyComponent.h"
#include "FormationState.h"

#include "Minigin.h"

bool CollisionManager::CollisionEffect(std::shared_ptr<Willem::GameObject> playersCollider, std::shared_ptr<Willem::GameObject> aliensCollider) // Returns true when it 100% removes something from a list
{
	if (playersCollider->HasTag("PlayerBullet") && !aliensCollider->HasTag("AlienBullet") && aliensCollider->HasTag("Alien") && !aliensCollider->HasTag("TractorBeam"))
	{
		if (!aliensCollider->HasTag("Boss") || aliensCollider->GetComponent<HealthComponent>()->GetHealthPoints() == 1)
		{
			if (aliensCollider->GetComponent<AIFlyComponent>()->CheckIfStateEqualsTemplate<FormationState>())
				playersCollider->GetParent().lock()->GetComponent<ScoreComponent>()->IncreaseScore(aliensCollider->GetName() + "Formation");
			else
				playersCollider->GetParent().lock()->GetComponent<ScoreComponent>()->IncreaseScore(aliensCollider->GetName() + "Diving");
		}



		Minigin::GetInstance().IncreaseBulletsHit();

		playersCollider->GetComponent<HealthComponent>()->Hit();
		aliensCollider->GetComponent<HealthComponent>()->Hit();

		return true;
	}

	else if (playersCollider->HasTag("Player") && aliensCollider->HasTag("TractorBeam") && !playersCollider->HasTag("PlayerBullet"))
	{
		if (aliensCollider->GetComponent<Willem::RenderComponent>()->GetSrcRect().y > 328)
		{
			auto controlComp = playersCollider->GetComponent<ControlComponent>();

			controlComp->SetEnabled(false);
			controlComp->EnableCaughtInTractorBeam(aliensCollider->GetParent());
			playersCollider->GetComponent<ShootComponent>()->SetEnabled(false);
		}
	}
	else if (playersCollider->HasTag("Player") && aliensCollider->HasTag("Alien") && !playersCollider->HasTag("PlayerBullet") && !aliensCollider->HasTag("TractorBeam"))
	{
		playersCollider->GetComponent<HealthComponent>()->Hit();
		aliensCollider->GetComponent<HealthComponent>()->Hit(2);

		return true;
	}

	return false;
}