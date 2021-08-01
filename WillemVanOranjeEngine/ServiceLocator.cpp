#include "AudioClasses.h"

using namespace Willem;

SoundSystem* ServiceLocator::m_pSsInstance;
NullSoundSystem ServiceLocator::m_NullSs;


SoundSystem& ServiceLocator::GetSoundSystem()
{
	if (m_pSsInstance)
		return *m_pSsInstance;
	else
		return m_NullSs;
}

void ServiceLocator::SetSoundSystem(SoundSystem* ss)
{
	m_pSsInstance = ss == nullptr ? &m_NullSs : ss;
}

void ServiceLocator::CleanUp()
{
	delete m_pSsInstance;
}