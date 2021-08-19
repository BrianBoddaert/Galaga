#include "Subject.h"


Willem::Subject::Subject()
{}

Willem::Subject::~Subject()
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		delete m_pObservers[i];
	}
}

void Willem::Subject::AddObserver(Observer* observer)
{
	m_pObservers.push_back(observer);
}

void Willem::Subject::RemoveObserver(Observer* observer)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
	{
		if (m_pObservers[i] == observer)
		{
			delete m_pObservers[i];
			m_pObservers[i] = nullptr;
		}
	}
}

void Willem::Subject::Notify(const Willem::GameObject* actor, const EVENT& event)
{
	for (size_t i = 0; i < m_pObservers.size(); i++)
		m_pObservers[i]->OnNotify(actor, event);
	
}
