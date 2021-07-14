#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Singleton.h"

namespace Willem
{
	enum class GameState
	{
		Playing,
		Paused,
		GameOver,
		PlayerDied
	};

	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, const int gameMode);

		void Update(float deltaT);
		void Render();
		std::shared_ptr<Scene> GetSceneByName(const std::string& n) const;
		std::shared_ptr<Scene> GetCurrentScene();
		void RemoveCurrentScene();
		//void SetCurrentSceneToNext();
		void SetCurrentScene(const std::shared_ptr<Scene>&);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_pScenes;
		std::shared_ptr<Scene> m_pCurrentScene = nullptr;
	};
}
