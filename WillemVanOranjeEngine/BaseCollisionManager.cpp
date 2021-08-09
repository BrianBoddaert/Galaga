#include "BaseCollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

using namespace Willem;

BaseCollisionManager::BaseCollisionManager()
	:m_ReCheckCollisions{false}
{}

void BaseCollisionManager::Update(float)
{
	m_ReCheckCollisions = true;

	while (m_ReCheckCollisions)
	{
		CheckCollisions();
	}

}

void BaseCollisionManager::CheckCollisions()
{
	m_ReCheckCollisions = false;

	for (size_t i = 0; i < m_pCollidersA.size(); i++)
	{
		for (size_t j = 0; j < m_pCollidersB.size(); j++)
		{
			if (IsColliding(m_pCollidersA[i], m_pCollidersB[j]))
			{
				if (CollisionEffect(m_pCollidersA[i], m_pCollidersB[j]))
				{
					m_ReCheckCollisions = true;
					return;
				}				
			}
		}
	}
	
}
void BaseCollisionManager::RemoveColliderByObject(GameObject* obj)
{
	for (size_t i = 0; i < m_pCollidersA.size(); i++)
	{
		if (m_pCollidersA[i].get() == obj)
		{
			m_pCollidersA.erase(m_pCollidersA.begin() + i);
			return;
		}
			
	}
	for (size_t i = 0; i < m_pCollidersB.size(); i++)
	{
		if (m_pCollidersB[i].get() == obj)
		{
			m_pCollidersB.erase(m_pCollidersB.begin() + i);
			return;
		}
			
	}
}
void BaseCollisionManager::RemoveColliderByObject(const std::shared_ptr<GameObject>& obj)
{
	for (size_t i = 0; i < m_pCollidersA.size(); i++)
	{
		if (m_pCollidersA[i] == obj)
			m_pCollidersA.erase(m_pCollidersA.begin() + i);
	}
	for (size_t i = 0; i < m_pCollidersB.size(); i++)
	{
		if (m_pCollidersB[i] == obj)
			m_pCollidersB.erase(m_pCollidersB.begin() + i);
	}
}
void BaseCollisionManager::AddCollider(const std::shared_ptr<GameObject>& gameObject)
{
	if (gameObject->HasTag("Alien") || gameObject->HasTag("AlienBullet"))
	{
		m_pCollidersB.push_back(gameObject);
	}
	else
	{
		m_pCollidersA.push_back(gameObject);
	}
}

bool BaseCollisionManager::IsColliding(std::shared_ptr<GameObject> colliderA, std::shared_ptr<GameObject> colliderB)
{
	const Vector3 pos1 = colliderA->GetComponent<TransformComponent>()->GetPosition();
	const Vector2 size1 = colliderA->GetComponent<RenderComponent>()->GetSpritePixelSizeScaled();
	const Vector3 pos2 = colliderB->GetComponent<TransformComponent>()->GetPosition();
	const Vector2 size2 = colliderB->GetComponent<RenderComponent>()->GetSpritePixelSizeScaled();

	const Vector2 TriggerOffset = {0,0 };

	if (pos1.x + size1.x + TriggerOffset.x >= pos2.x && pos1.y + size1.y + TriggerOffset.y >= pos2.y && !(pos1.x > pos2.x + size2.x + TriggerOffset.x) && !(pos1.y > pos2.y + size2.y + TriggerOffset.y))
	{
		return true;
	}
	else if (pos2.x + size2.x + TriggerOffset.x >= pos1.x && pos2.y + size2.y + TriggerOffset.y >= pos1.y && !(pos2.x > pos1.x + size1.x + TriggerOffset.x) && !(pos2.y > pos1.y + size1.y + TriggerOffset.y))
	{
		return true;
	}

	return false;
}

void BaseCollisionManager::RemoveCollidersByTag(const std::string& tag)
{
	for (size_t i = 0; i < m_pCollidersA.size(); i++)
	{
		if (m_pCollidersA[i]->HasTag(tag))
			m_pCollidersA.erase(m_pCollidersA.begin() + i);
	}

	for (size_t i = 0; i < m_pCollidersB.size(); i++)
	{
		if (m_pCollidersB[i]->HasTag(tag))
			m_pCollidersB.erase(m_pCollidersB.begin() + i);
	}
}

void BaseCollisionManager::RemoveCollidersByName(const std::string& name)
{
	for (size_t i = 0; i < m_pCollidersA.size(); i++)
	{
		if (m_pCollidersA[i]->GetName() == name)
			m_pCollidersA.erase(m_pCollidersA.begin() + i);
	}
	for (size_t i = 0; i < m_pCollidersB.size(); i++)
	{
		if (m_pCollidersB[i]->GetName() == name)
			m_pCollidersB.erase(m_pCollidersB.begin() + i);
	}
}

void BaseCollisionManager::ClearColliders()
{
	m_pCollidersA.clear();
	m_pCollidersB.clear();
}