#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include <memory>

namespace Willem
{
	class BaseCollisionManager
	{
	public:
		BaseCollisionManager();
		void Update(float deltaT);
		void AddCollider(const std::shared_ptr<GameObject>& gameObject);

		void RemoveColliderByObject(const std::shared_ptr<GameObject>& obj);
		void RemoveCollidersByTag(const Willem::Tag& tag);
		void RemoveCollidersByName(const std::string& name);
		void ClearColliders();
	protected:

		bool IsColliding(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2);
		virtual void CollisionEffect(std::shared_ptr<GameObject> player, std::shared_ptr<GameObject> collider) = 0;

		std::vector<std::shared_ptr<GameObject>> m_pColliders;
		std::vector<std::shared_ptr<GameObject>> m_pPlayers;
	};

}
