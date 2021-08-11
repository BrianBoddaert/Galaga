#include "SceneManager.h"
#include "Scene.h"

void Willem::SceneManager::Update(float deltaT)
{
	if (m_pCurrentScene)
	m_pCurrentScene->Update(deltaT);
}

void Willem::SceneManager::Render()
{
	if (m_pCurrentScene)
	m_pCurrentScene->Render();
}

Willem::Scene& Willem::SceneManager::CreateScene(const std::string& name,const int gameMode)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name,(GameMode)gameMode));
	m_pScenes.push_back(scene);

	m_pCurrentScene = scene;
	return *m_pCurrentScene;
}

void Willem::SceneManager::RemoveCurrentScene()
{
	for (size_t i = 0; i < m_pScenes.size(); i++)
	{
		if (m_pScenes[i] == m_pCurrentScene)
		m_pScenes.erase(m_pScenes.begin() + i);
	}

	m_pCurrentScene = nullptr;
}

std::shared_ptr<Willem::Scene> Willem::SceneManager::GetSceneByName(const std::string& n) const
{
	for (const auto& scene : m_pScenes)
	{
		if (scene->GetTag().compare(n) == 0)
			return scene;
	}
	return nullptr;
}

std::shared_ptr<Willem::Scene> Willem::SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}

void Willem::SceneManager::SetCurrentScene(const std::shared_ptr<Scene>& scene)
{
	m_pCurrentScene = scene;
}
