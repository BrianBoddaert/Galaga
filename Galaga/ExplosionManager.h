#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Structs.h"

class ExplosionManager final : public Willem::Singleton<ExplosionManager>
{
public:
	void SpawnExplosion(const Willem::Vector2& pos);
	void Update(float deltaT);
private:
	friend class Willem::Singleton<ExplosionManager>;
	
	const float m_ExplosionSpriteDuration = 0.1f;
	std::vector<std::pair< std::weak_ptr<Willem::GameObject>, float >> m_pExplosions;
};
