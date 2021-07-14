#pragma once
#include "GameObject.h"
#include "Observer.h"

namespace Willem
{
	
	class Subject
	{
		friend class GameObject;
		public:
			Subject();
			~Subject();
			Subject(const Subject&) = delete;
			Subject& operator=(const Subject&) = delete;
			Subject(Subject&&) = delete;
			Subject& operator= (Subject&&) = delete;

			void AddObserver(Observer* observer);
			void RemoveObserver(Observer* observer);

		protected:
			void Notify(const GameObject* actor, Event event);

		private:
			const static int m_MaxObserverCount = 3;
			unsigned int m_ObserverCount;

			Observer* m_pObservers[m_MaxObserverCount];

	};

}