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
	void LoadGameOverScreen(const Willem::GameMode& gm) const;
	void Cleanup();
	void ClearScreen();
	static void IncreaseBulletsFired() { AmountOfBulletsFired++; }
	static void ResetBulletsFired() { AmountOfBulletsFired = 0; }
	static void IncreaseBulletsHit() { AmountOfBulletsHit++; }
	static void ResetBulletsHit() { AmountOfBulletsHit = 0; }
	void Run();
	void LoadSceneByGameMode(Willem::GameMode gameMode) const;
	static SDL_Surface* GetWindowSurface() { return m_WindowSurface; }

private:
	friend class Willem::Singleton<Minigin>;

	static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps		
	static float MsPerUpdate;
	static int AmountOfBulletsFired;
	static int AmountOfBulletsHit;
	SDL_Window* m_Window{};
	static SDL_Surface* m_WindowSurface;

};
