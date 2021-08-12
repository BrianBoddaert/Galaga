#pragma once
#include "SceneManager.h"
#include <memory>
#include "GameObject.h"

namespace Willem
{
	enum class GameMode
	{
		SinglePlayer,
		CoOp,
		Versus
	};

	class SceneObject;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name,const int gameMode);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update(float deltaT);
		void Render() const;
		const std::string& GetTag() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::shared_ptr<GameObject> GetObjectByName(const std::string& name) const;
		std::shared_ptr<GameObject> GetObjectByObject(GameObject*) const;

		std::vector<std::shared_ptr<GameObject>> GetObjectsByTag(const std::string& tag) const;
		void AddPlayer(const std::shared_ptr<GameObject>& player);
		std::shared_ptr<GameObject> GetPlayer(int index) const;
		std::shared_ptr<GameObject> GetCurrentMap() const;
		const std::vector< std::shared_ptr<GameObject>>& GetPlayers() const {return m_pPlayers;}
		void RemoveObjectsByTag(const std::string& tag);
		void RemovePlayersByTag(const std::string& tag);
		void RemoveObjectsByName(const std::string& name);
		void RemoveObjectsByObject(GameObject* obj);
		void ClearObjects();
		const GameMode& GetGameMode() const { return m_GameMode; };
		void SetGameModeToNext();
	private: 
		explicit Scene(const std::string& name, const GameMode& gameMode);
		GameMode m_GameMode;
		const std::string m_Name;

		std::vector< std::shared_ptr<GameObject>> m_pPlayers{};
		std::vector < std::shared_ptr<GameObject>> m_pObjects{};
		std::shared_ptr<GameObject> m_pCurrentMap;
	};

}
