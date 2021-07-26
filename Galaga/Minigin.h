#pragma once
#include <memory>
#include <SDL.h>
#include <stdio.h>
#include "Structs.h"
#include "Singleton.h"
#include "Scene.h"
//struct SDL_Window;



class Minigin final : public Willem::Singleton<Minigin>
{
public:
	void Initialize();
	void AssignKeys();
	void LoadGame() const;
	void LoadSinglePlayerScene() const;
	void LoadCoOpScene() const;
	void LoadVersusScene() const;
	void LoadHUD(Willem::Scene& scene) const;
	void Cleanup();
	void Run();
	bool SetPaused(bool v);
	bool GetGameOver() const { return m_Gamestate == Willem::GameState::GameOver; }
	bool GetPaused() const { return m_Gamestate == Willem::GameState::Paused; }
	void SetGameState(const Willem::GameState& state) { m_Gamestate = state; }
	const Willem::GameState& GetGameState() const { return m_Gamestate; }
	void LoadSceneByGameMode(Willem::GameMode gameMode) const;
	void ClearGame();
	static SDL_Surface* GetWindowSurface() { return m_WindowSurface; }

private:
	friend class Willem::Singleton<Minigin>;




	static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
	static float MsPerUpdate;
	Willem::GameState m_Gamestate;
	SDL_Window* m_Window{};
	static SDL_Surface* m_WindowSurface;
};
