#pragma once
#include <iostream>
#include "SceneObject.h"
#include <vector>


namespace Willem
{
	enum class Event
	{
		ActorDied,
		ScoreSet,
		ColorChanged = 25,
		CoilyHitWithFlyingDisc = 500,
		DiscLeftAtEndOfTheStage = 50,
		CatchSlickOrSam = 300
	};

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
		void Notify(Event event);

		template<typename T>
		T* GetComponent() const
		{
			for (Component * c: m_Components)
			{
				if (dynamic_cast<T*>(c))
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
	};
}
