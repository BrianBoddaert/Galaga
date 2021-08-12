#pragma once
#include <iostream>
#include "SceneObject.h"
#include <vector>


namespace Willem
{
#define EVENT std::string

	class Observer;
	class Subject;
	class Component;
	class Texture2D;

	class GameObject final : public SceneObject
	{
	public:
		void Update(float deltaT) override;
		void Render() const override;
		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y, float z);

		void AddComponent(Component*);
		void AddWatcher(Observer*);

		void SetEnabled(bool);
		void Notify(const EVENT& event);

		void SetParent(std::weak_ptr<GameObject> go) { m_pParent = go; };
		std::weak_ptr<GameObject> GetParent() const {return m_pParent;};

		template<typename T>
		T* GetComponent() const
		{
			for (Component * c: m_Components)
			{
				if (c && dynamic_cast<T*>(c))
					return (T*)c;
			}
			return nullptr;
		}

		GameObject(const std::string& name);
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		bool m_Enabled;
		std::vector<Component*> m_Components;
		Subject* m_pActorChanged;
		std::weak_ptr<GameObject> m_pParent;
	};
}
