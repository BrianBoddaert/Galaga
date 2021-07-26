#include "BaseCollisionManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TransformComponent.h"

using namespace Willem;

BaseCollisionManager::BaseCollisionManager()
{}

void BaseCollisionManager::Update(float)
{

	for (size_t i = 0; i < m_pPlayers.size(); i++)
	{
		for (size_t j = 0; j < m_pColliders.size(); j++)
		{
			if (IsColliding(m_pPlayers[i], m_pColliders[j]))
			{
				CollisionEffect(m_pPlayers[i], m_pColliders[j]);
			}
		}
	}

}

void BaseCollisionManager::RemoveColliderByObject(const std::shared_ptr<GameObject>& obj)
{
	for (size_t i = 0; i < m_pColliders.size(); i++)
	{
		if (m_pColliders[i] == obj)
			m_pColliders.erase(m_pColliders.begin() + i);
	}
}
void BaseCollisionManager::AddCollider(const std::shared_ptr<GameObject>& gameObject)
{
	if (gameObject->HasTag("Player"))
	{
		m_pPlayers.push_back(gameObject);
	}
	else
	{
		m_pColliders.push_back(gameObject);
	}
}

bool BaseCollisionManager::IsColliding(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider)
{
	const Vector3 pos1 = player->GetComponent<TransformComponent>()->GetPosition();
	const Vector2 size1 = player->GetComponent<RenderComponent>()->GetSpritePixelSizeScaled();
	const Vector3 pos2 = collider->GetComponent<TransformComponent>()->GetPosition();
	const Vector2 size2 = collider->GetComponent<RenderComponent>()->GetSpritePixelSizeScaled();

	const Vector2 TriggerOffset = { -(size2.x/1.75f),-(size2.y/ 1.75f) };

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
	for (size_t i = 0; i < m_pColliders.size(); i++)
	{
		if (m_pColliders[i]->HasTag(tag))
			m_pColliders.erase(m_pColliders.begin() + i);
	}
}

void BaseCollisionManager::RemoveCollidersByName(const std::string& name)
{
	for (size_t i = 0; i < m_pColliders.size(); i++)
	{
		if (m_pColliders[i]->GetName() == name)
			m_pColliders.erase(m_pColliders.begin() + i);
	}
}

void BaseCollisionManager::ClearColliders()
{
	m_pPlayers.clear();
	m_pColliders.clear();
}