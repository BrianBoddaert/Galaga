#include <iostream>
#include <algorithm>
#include "Scene.h"
#include "SceneManager.h"
#include "TransformComponent.h"

using namespace Willem;

Scene::Scene(const std::string& name,const GameMode& gameMode)
	: m_Name(name)
	, m_GameMode{gameMode}
{
}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_pObjects.push_back(object);
}

void Scene::Update(float deltaT)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		m_pObjects[i]->Update(deltaT);
	}

}


void Scene::Render() const
{

	//for (const auto& player : m_pPlayers)
	//{
	//		player->Render();
	//}
	for (const auto& object : m_pObjects)
	{
		object->Render();
	}
}

const std::string& Scene::GetTag() const
{
	return m_Name;
}

std::shared_ptr<Willem::GameObject> Scene::GetObjectByName(const std::string& name) const
{
	for (const auto& object : m_pObjects)
	{
		if (object->GetName().compare(name) == 0)
		return object;
	}
	return nullptr;
}

std::shared_ptr<Willem::GameObject> Scene::GetObjectByObject(GameObject* go) const
{
	for (const auto& object : m_pObjects)
	{
		if (object.get() == go)
			return object;
	}
	return nullptr;
}
void Scene::AddPlayer(const std::shared_ptr<GameObject>& player)
{
	m_pPlayers.push_back(player);
	m_pObjects.push_back(player);
}

std::shared_ptr<Willem::GameObject> Scene::GetPlayer(int index) const
{
	if (m_pPlayers.size() <= (unsigned)index)
		return nullptr;

	return m_pPlayers[index];
}


std::shared_ptr<Willem::GameObject> Scene::GetCurrentMap() const
{
	return m_pCurrentMap;
}
void Scene::RemoveObjectsByTag(const std::string &tag)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i]->HasTag(tag))
			m_pObjects.erase(m_pObjects.begin() + i);
	}
}

void Scene::RemovePlayersByTag(const std::string& tag)
{
	for (size_t i = 0; i < m_pPlayers.size(); i++)
	{
		if (m_pPlayers[i]->HasTag(tag))
			m_pPlayers.erase(m_pPlayers.begin() + i);
	}
}

void Scene::RemoveObjectsByObject(GameObject* obj)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i].get() == obj)
			m_pObjects.erase(m_pObjects.begin() + i);
	}
}
void Scene::ClearObjects()
{
	m_pObjects.clear();
}

void Scene::RemoveObjectsByName(const std::string& name)
{
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i]->GetName() == name)
			m_pObjects.erase(m_pObjects.begin() + i);
	}
}

void Scene::SetGameModeToNext()
{
	if (m_GameMode == GameMode::Versus)
		m_GameMode = GameMode::SinglePlayer;
	else
		m_GameMode = GameMode((int)m_GameMode + 1);

}

std::vector<std::shared_ptr<Willem::GameObject>> Scene::GetObjectsByTag(const std::string& tag) const
{
	std::vector<std::shared_ptr<GameObject>> objects;
	for (size_t i = 0; i < m_pObjects.size(); i++)
	{
		if (m_pObjects[i]->HasTag(tag))
			objects.push_back(m_pObjects[i]);
	}

	return objects;
}