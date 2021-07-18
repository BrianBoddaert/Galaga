#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include "GameObject.h"
#include "Structs.h"
#pragma warning(pop)

namespace Willem
{
	class Font;
	class Texture2D;
	class Component
	{
	public:

		virtual void Update(float) {};
		virtual void Render(const Willem::Vector2&, const Willem::Vector2&) const {};
		virtual ~Component() {};

		void SetGameObject(GameObject* obj) { m_pGameObject = obj; }
		GameObject* GetGameObject() { return m_pGameObject; }

	private:
	protected:
		GameObject* m_pGameObject;
	};

}