#include "stdafx.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "SceneManager.h"

void CollisionManager::CollisionEffect(std::shared_ptr<Willem::GameObject> player, std::shared_ptr<Willem::GameObject> collider)
{

	//if (player->GetComponent<MoveComponent>()->GetIsOnDisc() || player->GetComponent<MoveComponent>()->IsFallingToDeath())
	//	return;

	//auto colliderMoveComp = collider->GetComponent<MoveComponent>();
	//if (colliderMoveComp)
	//	if (colliderMoveComp->IsFallingToDeath())
	//		return;

	//if (collider->HasTag(Willem::Tag::Coily) || collider->HasTag(Willem::Tag::WrongWay))
	//{
	//	player->GetComponent<HealthComponent>()->Die();

	//	//EnemyManager::GetInstance().Reset();
	//}
	//else if (collider->HasTag(Willem::Tag::SlickSam))
	//{
	//	collider->GetComponent<HealthComponent>()->Die();
	//	auto scoreComponent = player->GetComponent<ScoreComponent>();
	//	scoreComponent->IncreaseScore(Willem::Event::CatchSlickOrSam);
	//	
	//}
	//else if (collider->HasTag(Willem::Tag::Disc))
	//{
	//	auto moveComp = player->GetComponent<MoveComponent>();
	//	if (!moveComp->GetJumpingOnDisc())
	//		return;

	//	moveComp->SetDiscGameObject(collider);
	//	Willem::SceneManager::GetInstance().GetCurrentScene()->GetCurrentMap()->GetComponent<MapComponent>()->GetDiscByGameObject(collider)->SetMoving(true);
	//}
}