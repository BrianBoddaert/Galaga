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
		void CheckCollisions();
		void AddCollider(const std::shared_ptr<GameObject>& gameObject);

		void RemoveColliderByObject(const std::shared_ptr<GameObject>& obj);
		void RemoveColliderByObject(GameObject * obj);
		void RemoveCollidersByTag(const std::string& tag);
		void RemoveCollidersByName(const std::string& name);
		void ClearColliders();
	protected:

		bool IsColliding(std::shared_ptr<GameObject> obj1, std::shared_ptr<GameObject> obj2);
		virtual bool CollisionEffect(std::shared_ptr<GameObject> colliderA, std::shared_ptr<GameObject> colliderB) = 0;

		std::vector<std::shared_ptr<GameObject>> m_pCollidersA;
		std::vector<std::shared_ptr<GameObject>> m_pCollidersB;

		bool m_ReCheckCollisions;
	};

}
