#pragma once
#include "BaseCollisionManager.h"
#include "Singleton.h"

class CollisionManager final : public Willem::Singleton<CollisionManager>,  public Willem::BaseCollisionManager
{
private:
	friend class Willem::Singleton<CollisionManager>;

	bool CollisionEffect(std::shared_ptr<Willem::GameObject> player, std::shared_ptr<Willem::GameObject> collider) override;
};

