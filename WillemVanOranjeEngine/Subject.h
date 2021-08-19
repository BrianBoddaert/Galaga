#pragma once
#include "GameObject.h"
#include "Observer.h"

namespace Willem
{
	
	class Subject
	{
		public:
			Subject();
			~Subject();
			Subject(const Subject&) = delete;
			Subject& operator=(const Subject&) = delete;
			Subject(Subject&&) = delete;
			Subject& operator= (Subject&&) = delete;

			void AddObserver(Observer* observer);
			void RemoveObserver(Observer* observer);

			void Notify(const GameObject* actor, const EVENT& event);			

		private:
			std::vector<Observer*> m_pObservers;

	};

}