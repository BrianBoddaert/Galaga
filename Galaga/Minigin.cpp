#include "stdafx.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <thread>
//#include <SDL.h>
#include <SDL_mixer.h>

#include "GameObject.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "MoveCommand.h"
#include "ControlComponent.h"


#include "TimerComponent.h"
#include "AudioClasses.h"
#include <Xinput.h>
#include <glm\vec2.hpp>
#include "CollisionManager.h"
#include "ShootCommand.h"
#include "ShootComponent.h"
#include "EnemyManager.h"
#include "ExplosionManager.h"
#include "HealthComponent.h"

using namespace std;
using namespace std::chrono;
using namespace Willem;

float Minigin::MsPerUpdate = 0.02f;
SDL_Surface* Minigin::m_WindowSurface = nullptr;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Video Error: ") + SDL_GetError());


	if (SDL_Init(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init_Audio Error: ") + SDL_GetError());

	int frequency = 44100;
	int chunkSize = 2048;
	int channels = 2;

	if (Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, channels, chunkSize) < 0)
		throw std::runtime_error(std::string("SDL_Audio Error: ") + Mix_GetError());

	//Mix_Music* bgm = Mix_LoadMUS("music.mp3"); // Wav, mp3 both work
	//Mix_Chunk* soundEffect = Mix_LoadWAV("soundEffect.wav"); // Wav files

	m_Window = SDL_CreateWindow(
		"Galaga",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		550,
		SDL_WINDOW_OPENGL
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);

	m_WindowSurface = SDL_GetWindowSurface(m_Window);

	Willem::ServiceLocator::SetSoundSystem(new LoggingSoundSystem(new SdlSoundSystem()));
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Jump, "../Data/Audio/jump.mp3");
	//Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Fall, "../Data/Audio/fall.mp3");
	//Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Lift, "../Data/Audio/lift.mp3");
	//Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary(EffectId::Victory, "../Data/Audio/victory.mp3");
	//ServiceLocator::GetSoundSystem().AddSoundToLibrary(MusicId::Ambient, "../Data/royaltyFreeSong.mp3");

	//ServiceLocator::GetSoundSystem().QueueSound(MusicId::Ambient,0.3f);
	////Mix_Music* music = Mix_LoadMUS("../Data/mortalkombat.mp3"); // Wav, mp3 both work

	//ServiceLocator::GetSoundSystem().QueueSound(EffectId::Jump, 1.0f);

}


void Minigin::AssignKeys()
{
	auto& input = InputManager::GetInstance();

	//for (int i = 0; i < XUSER_MAX_COUNT; i++)
	//{
	//	//input.AssignControllerKey<DieCommand>(ControllerButton::LeftThumbStickUp, i);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickUp, i, (int)MoveInputDirections::Up);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickLeft, i, (int)MoveInputDirections::Left);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickRight, i, (int)MoveInputDirections::Right);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickDown, i, (int)MoveInputDirections::Down);

	//	input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonUp, i, (int)MoveInputDirections::Up);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonLeft, i, (int)MoveInputDirections::Left);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonRight, i, (int)MoveInputDirections::Right);
	//	input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonDown, i, (int)MoveInputDirections::Down);

	//	input.AssignControllerKey<PauseCommand>(ControllerButton::ButtonStart, i);
	//	input.AssignControllerKey<SwitchSceneCommand>(ControllerButton::TriggerRight, i);
	//}

	//input.AssignKeyboardKey<PauseCommand>(KeyboardButton::P);

	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::A, (int)MoveInputDirections::Left);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::D, (int)MoveInputDirections::Right);
	input.AssignKeyboardKey<ShootCommand>(KeyboardButton::SPACE, 0);

	//input.AssignKeyboardKey<SwitchSceneCommand>(KeyboardButton::PLUS, 0);
}

void Minigin::LoadSinglePlayerScene() const
{

	auto& scene = SceneManager::GetInstance().CreateScene("SinglePlayerScene", (int)GameMode::SinglePlayer);
	LoadHUD(scene);

	auto player = std::make_shared<GameObject>("Player1");

	SDL_Rect playerSrcRect = { 109,1,16,16 };

	const float offsetFromBottom = 50.0f;
	const Willem::Vector2 playerHalfSize = { playerSrcRect.w / 2.0f,playerSrcRect.h / 2.0f };
	const Willem::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h - float(playerSrcRect.h * GAMESCALE) - offsetFromBottom,1 };

	player->AddComponent(new ControlComponent(playerPos));

	player->AddComponent(new RenderComponent(playerSrcRect));
	player->SetTexture("Galaga2.png");

	player->AddComponent(new TransformComponent(playerPos, float(GAMESCALE)));
	player->AddComponent(new ShootComponent());
	player->AddComponent(new HealthComponent(3));						
	//player->AddComponent(new ScoreComponent(0));							<<< UNCOMMENT

	//player->AddWatcher(new LivesObserver());								<<< UNCOMMENT
	//player->AddWatcher(new ScoreObserver());								<<< UNCOMMENT
	player->AddTag("Player");
	player->AddTag("Player1");
	CollisionManager::GetInstance().AddCollider(player);					
	scene.AddPlayer(player);
// 
	//{
	//	auto map = std::make_shared<GameObject>("Map");
	//	//250,200
	//	const Willem::Vector3 highestCubePos = { float(m_WindowSurface->w / 2), float(m_WindowSurface->h / 2),0 };
	//	map->AddComponent(new MapComponent(scene, highestCubePos));
	//	map->AddWatcher(new LevelCompleteObserver());
	//	scene.AddMap(map);
	//}
	//{
	//	auto player = std::make_shared<GameObject>("Player1");
	//	const Willem::Vector2 playerHalfSize = { 8,8 };
	//	const Willem::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h / 2 - playerHalfSize.y,1 };
	//	ControlComponent* controlComponent = new ControlComponent(playerPos);  // ControlComponent();

	//	player->AddComponent(controlComponent);

	//	SDL_Rect playerSrcRect = { 0,0,16,16 };

	//	player->AddComponent(new RenderComponent(playerSrcRect));
	//	player->SetTexture("Textures/Qbert2.png");

	//	player->AddComponent(new TransformComponent(playerPos, 1.0f));
	//	//player->SetPosition(playerPos.x, playerPos.y);

	//	player->AddComponent(new HealthComponent(3));
	//	player->AddComponent(new ScoreComponent(0));

	//	player->AddWatcher(new LivesObserver());
	//	player->AddWatcher(new ScoreObserver());
	//	player->AddComponent(new MoveComponent(0));
	//	player->AddTag(Tag::Player);
	//	player->AddTag(Tag::Player1);
	//	CollisionManager::GetInstance().AddCollider(player);
	//	scene.AddPlayer(player);
	//}

	//scene.SortOnZAxis();
}

void Minigin::LoadCoOpScene() const
{

	//auto& scene = SceneManager::GetInstance().CreateScene("CoOpScene", (int)GameMode::CoOp);

	//LoadHUD(scene);
	//auto map = std::make_shared<GameObject>("Map");
	////250,200
	//const Willem::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	//MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	//map->AddComponent(mapComp);
	//map->AddWatcher(new LevelCompleteObserver());
	//scene.AddMap(map);
	//
	//{

	//	{
	//		auto player = std::make_shared<GameObject>("Player1");

	//		const Willem::Vector2 playerHalfSize = { 8,8 };
	//		const Willem::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) - 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,7 };

	//		ControlComponent* controlComponent = new ControlComponent(playerPos);

	//		player->AddComponent(controlComponent);
	//		SDL_Rect playerSrcRect = { 0,0,16,16 };

	//		player->AddComponent(new RenderComponent(playerSrcRect));
	//		player->SetTexture("Textures/Qbert2.png");
	//		//const Willem::Vector2& cubeOffset =  mapComp->GetCubeOffset();

	//		player->AddComponent(new TransformComponent(playerPos, 1.0f));

	//		player->AddComponent(new HealthComponent(3));
	//		player->AddComponent(new ScoreComponent(0));

	//		player->AddWatcher(new LivesObserver());
	//		player->AddWatcher(new ScoreObserver());
	//		player->AddComponent(new MoveComponent(27));
	//		player->AddTag(Tag::Player);
	//		player->AddTag(Tag::Player1);
	//		CollisionManager::GetInstance().AddCollider(player);
	//		scene.AddPlayer(player);
	//	}
	//	{
	//		auto player = std::make_shared<GameObject>("Player2");

	//		SDL_Rect playerSrcRect = { 0,0,16,16 };
	//		const Willem::Vector2 playerHalfSize = { 8,8 };
	//		player->AddComponent(new RenderComponent(playerSrcRect));
	//		player->SetTexture("Textures/Qbert2.png");
	//		//const Willem::Vector2& cubeOffset =  mapComp->GetCubeOffset();
	//		const Willem::Vector3 playerPos = { (m_WindowSurface->w / 2 + playerHalfSize.x) + 96, (m_WindowSurface->h / 2 - playerHalfSize.y) + 144,7 };
	//		ControlComponent* controlComponent = new ControlComponent(playerPos);

	//		player->AddComponent(controlComponent);

	//		player->AddComponent(new TransformComponent(playerPos, 1.0f));

	//		player->AddComponent(new HealthComponent(3));
	//		player->AddComponent(new ScoreComponent(0));

	//		player->AddWatcher(new LivesObserver());
	//		player->AddWatcher(new ScoreObserver());
	//		player->AddComponent(new MoveComponent(6));
	//		player->AddTag(Tag::Player);
	//		player->AddTag(Tag::Player2);
	//		CollisionManager::GetInstance().AddCollider(player);
	//		scene.AddPlayer(player);
	//	}
	//}
	//scene.SortOnZAxis();
}

void Minigin::LoadVersusScene() const
{

	//auto& scene = SceneManager::GetInstance().CreateScene("VersusScene", (int)GameMode::Versus);
	//LoadHUD(scene);
	//auto map = std::make_shared<GameObject>("Map");
	////250,200
	//const Willem::Vector3 highestCubePos = { (float)m_WindowSurface->w / 2,  (float)m_WindowSurface->h / 2,0 };
	//MapComponent* mapComp = new MapComponent(scene, highestCubePos);
	//map->AddComponent(mapComp);
	//map->AddWatcher(new LevelCompleteObserver());
	//scene.AddMap(map);

	//{

	//	{
	//		auto player = std::make_shared<GameObject>("Player1");

	//		const Willem::Vector2 playerHalfSize = { 8,8 };
	//		const Willem::Vector3 playerPos = { m_WindowSurface->w / 2 + playerHalfSize.x, m_WindowSurface->h / 2 - playerHalfSize.y,0 };

	//		ControlComponent* controlComponent = new ControlComponent(playerPos);

	//		player->AddComponent(controlComponent);
	//		SDL_Rect playerSrcRect = { 0,0,16,16 };

	//		player->AddComponent(new RenderComponent(playerSrcRect));
	//		player->SetTexture("Textures/Qbert2.png");
	//		//const Willem::Vector2& cubeOffset =  mapComp->GetCubeOffset();

	//		player->AddComponent(new TransformComponent(playerPos, 1.0f));

	//		player->AddComponent(new HealthComponent(3));
	//		player->AddComponent(new ScoreComponent(0));

	//		player->AddWatcher(new LivesObserver());
	//		player->AddWatcher(new ScoreObserver());
	//		player->AddComponent(new MoveComponent(0));
	//		player->AddTag(Tag::Player);
	//		player->AddTag(Tag::Player1);
	//		CollisionManager::GetInstance().AddCollider(player);
	//		scene.AddPlayer(player);
	//	}
	//}
	//scene.SortOnZAxis();
}

void Minigin::LoadHUD(Willem::Scene& scene) const
{
	// m_WindowSurface;
	{
		auto oneUpHUD = std::make_shared<GameObject>("OneUpHUD");
		oneUpHUD->AddComponent(new TransformComponent({ 20,5.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		oneUpHUD->AddComponent(new RenderComponent());
		oneUpHUD->AddComponent(new Willem::TextComponent(oneUpHUD.get(), "1UP", font, { 255,0,0 }));

		scene.Add(oneUpHUD);
	}

	{
		auto scoreHUD = std::make_shared<GameObject>("ScoreHUD");
		scoreHUD->AddComponent(new TransformComponent({ 20,25.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		scoreHUD->AddComponent(new RenderComponent());
		scoreHUD->AddComponent(new Willem::TextComponent(scoreHUD.get(), "0", font, { 255,255,255 }));

		scene.Add(scoreHUD);
	}

	{
		auto lifeCounterOne = std::make_shared<GameObject>("LifeCounter1");
		const SDL_Rect srcRect = { 109,1,16,16 };
		const float offset = 5.0f;
		lifeCounterOne->AddComponent(new TransformComponent({ offset,float(m_WindowSurface->h - srcRect.h * GAMESCALE - offset),10 }, GAMESCALE));
		lifeCounterOne->AddComponent(new RenderComponent(srcRect));
		lifeCounterOne->SetTexture("Galaga2.png");
		scene.Add(lifeCounterOne);
	}
	{
		auto lifeCounterTwo = std::make_shared<GameObject>("LifeCounter2");
		const SDL_Rect srcRect = { 109,1,16,16 };
		const float offset = 5.0f;
		lifeCounterTwo->AddComponent(new TransformComponent({ offset + srcRect.w * GAMESCALE,float(m_WindowSurface->h - srcRect.h * GAMESCALE - offset),10 }, GAMESCALE));
		lifeCounterTwo->AddComponent(new RenderComponent(srcRect));
		lifeCounterTwo->SetTexture("Galaga2.png");
		scene.Add(lifeCounterTwo);
	}
}

void Minigin::LoadSceneByGameMode(Willem::GameMode gamemode) const
{
	if (gamemode == Willem::GameMode::SinglePlayer)
		LoadSinglePlayerScene();
	else if (gamemode == Willem::GameMode::CoOp)
		LoadCoOpScene();
	else if (gamemode == Willem::GameMode::Versus)
		LoadVersusScene();
}

void Minigin::LoadGame() const
{

	//LoadCoOpScene();
	//LoadVersusScene();
	LoadSinglePlayerScene();

}

void Minigin::Cleanup()
{
	ServiceLocator::CleanUp();

	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);


	m_Window = nullptr;
	Mix_Quit();
	SDL_Quit();

}

void Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	AssignKeys();
	LoadGame();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& enemyManager = EnemyManager::GetInstance();
	auto& collisionManager = CollisionManager::GetInstance();
	auto& explosionManager = ExplosionManager::GetInstance();
	auto& input = InputManager::GetInstance();

	bool doContinue = true;
	auto lastTime = high_resolution_clock::now();
	float lag = 0.0f;
	m_Gamestate = GameState::Playing;

	std::thread audioThread(&SoundSystem::Update, &ServiceLocator::GetSoundSystem());

	while (doContinue)
	{

		const auto currentTime = high_resolution_clock::now();
		float deltaTime = duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();

		while (lag >= MsPerUpdate)
		{
			if (m_Gamestate == GameState::Playing)
			{
				sceneManager.Update(deltaTime);
				enemyManager.Update(deltaTime);
				collisionManager.Update(deltaTime);
				explosionManager.Update(deltaTime);
			}
			lag -= MsPerUpdate;
		}

		//audioThread.join();

		renderer.Render();

		auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
		this_thread::sleep_for(sleepTime);
	}

	audioThread.detach();
	Cleanup();
}


void Minigin::ClearGame()
{
	//EnemyManager::GetInstance().Reset();
	CollisionManager::GetInstance().ClearColliders();

	auto& sceneManager = Willem::SceneManager::GetInstance();
	auto scene = sceneManager.GetCurrentScene();
	scene->ClearObjects();
	m_Gamestate = GameState::Playing;
	Willem::GameMode gamemode = scene->GetGameMode();
	sceneManager.RemoveCurrentScene();

	LoadSceneByGameMode(gamemode);
}

bool Minigin::SetPaused(bool v)
{
	if (v)
	{
		if (m_Gamestate == Willem::GameState::Playing)
		{
			m_Gamestate = Willem::GameState::Paused;
			return true;
		}

	}
	else
	{
		if (m_Gamestate == Willem::GameState::Paused)
		{
			m_Gamestate = Willem::GameState::Playing;
			return true;
		}

	}

	return false;

}

