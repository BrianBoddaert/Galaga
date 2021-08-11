#include "stdafx.h"
#include "ExplosionManager.h"
#include <SDL.h>
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace Willem;

void ExplosionManager::SpawnExplosion(const Willem::Vector2& pos)
{
	auto explosion = std::make_shared<Willem::GameObject>("Explosion");

	const SDL_Rect srcRect = { 289,1,32,32 };

	explosion->AddComponent(new RenderComponent(srcRect));
	explosion->SetTexture("Galaga2.png");
	explosion->AddComponent(new TransformComponent(Vector3{ pos.x - float(srcRect.w /2),pos.y - float(srcRect.h / 2),1.0f }, float(GAMESCALE)));

	explosion->AddTag("Explosion");

	SceneManager::GetInstance().GetCurrentScene()->Add(explosion);

	std::weak_ptr<Willem::GameObject> weakPtr = explosion;
	m_pExplosions.push_back(std::make_pair(weakPtr, m_ExplosionSpriteDuration));
}

void ExplosionManager::Update(float deltaT)
{
	for (size_t i = 0; i < m_pExplosions.size();)
	{
		m_pExplosions[i].second -= deltaT;
		if (m_pExplosions[i].second < 0)
		{
			RenderComponent* renderComp = m_pExplosions[i].first.lock().get()->GetComponent<RenderComponent>();
			SDL_Rect srcRect = renderComp->GetSrcRect();

			if (srcRect.x == 425)
			{
				SceneManager::GetInstance().GetCurrentScene()->RemoveObjectsByObject(m_pExplosions[i].first.lock().get());
				m_pExplosions.erase(m_pExplosions.begin() + i++);
			}
			else
			{
				m_pExplosions[i].second = m_ExplosionSpriteDuration;


				const int offsetWidth = 2;
				srcRect.x += offsetWidth + srcRect.w;
				renderComp->SetSrcRect(srcRect);
				++i;
			}

		}
		else
		{
			++i;
		}

	}

}

void ExplosionManager::Clear()
{
	m_pExplosions.clear();
}