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
#include "ScoreComponent.h"
#include "ScoreObserver.h"

#include "SwitchGameModeCommand.h"
#include "GameOverObserver.h"
#include "BossCommand.h"
#include "BackgroundComponent.h"

using namespace std;
using namespace std::chrono;
using namespace Willem;

float Minigin::MsPerUpdate = 0.02f;
int Minigin::AmountOfBulletsFired = 0;
int Minigin::AmountOfBulletsHit= 0;
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
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("TractorBeam", "../Data/Audio/TractorBeam.mp3", false);
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("PlayerShoot", "../Data/Audio/PlayerShoot.mp3",false);
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("EnemyDies", "../Data/Audio/EnemyDies.mp3", false);
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("PlayerDies", "../Data/Audio/PlayerDies.mp3", false);
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("CapturedShip", "../Data/Audio/CapturedShip.mp3", false);
	Willem::ServiceLocator::GetSoundSystem().AddSoundToLibrary("StartGame", "../Data/Audio/Start.mp3", false);

}


void Minigin::AssignKeys()
{
	auto& input = InputManager::GetInstance();

	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::LeftThumbStickRight, i, (int)MoveInputDirections::Right);

		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonLeft, i, (int)MoveInputDirections::Left);
		input.AssignControllerKey<MoveCommand>(ControllerButton::ButtonRight, i, (int)MoveInputDirections::Right);

		//input.AssignControllerKey<PauseCommand>(ControllerButton::ButtonStart, i);
		input.AssignControllerKey<SwitchGameModeCommand>(ControllerButton::TriggerRight, i);

		input.AssignControllerKey<ShootCommand>(ControllerButton::ButtonA, i, 0);

		input.AssignControllerKey<BossCommand>(ControllerButton::ButtonX, i, (int)BossMoves::Shoot);
		input.AssignControllerKey<BossCommand>(ControllerButton::ButtonY, i, (int)BossMoves::Dive);
		input.AssignControllerKey<BossCommand>(ControllerButton::ButtonB, i, (int)BossMoves::TractorBeam);
	}

	//input.AssignKeyboardKey<PauseCommand>(KeyboardButton::P);

	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::A, (int)MoveInputDirections::Left);
	input.AssignKeyboardKey<MoveCommand>(KeyboardButton::D, (int)MoveInputDirections::Right);
	input.AssignKeyboardKey<SwitchGameModeCommand>(KeyboardButton::PLUS,0);
	input.AssignKeyboardKey<ShootCommand>(KeyboardButton::SPACE, 0);

	//input.AssignKeyboardKey<SwitchSceneCommand>(KeyboardButton::PLUS, 0);
}

void Minigin::LoadSinglePlayerScene() const
{
	ServiceLocator::GetSoundSystem().QueueSound("StartGame", false, 0.1f);

	auto& scene = SceneManager::GetInstance().CreateScene("SinglePlayerScene", (int)GameMode::SinglePlayer);
	LoadHUD(scene);

	auto player = std::make_shared<Willem::GameObject>("Player1");

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
	player->AddComponent(new ScoreComponent(0));

	player->AddWatcher(new ScoreObserver());
	player->AddWatcher(new GameOverObserver());
	player->AddTag("Player");
	player->AddTag("Player1");
	CollisionManager::GetInstance().AddCollider(player);					
	scene.AddPlayer(player);

}

void Minigin::LoadGameOverScreen(const Willem::GameMode& gameMode) const
{
	auto& scene = SceneManager::GetInstance().CreateScene("GameOverScene", (int)gameMode);
	float startY = 200.0f;
	float offsetX = 200.0f;
	float offsetY = 30.0f;
	float xPos = float(m_WindowSurface->w / 3);

	{
		auto results = std::make_shared<Willem::GameObject>("Results");
		results->AddComponent(new TransformComponent({ xPos + 50.0f,startY,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		results->AddComponent(new RenderComponent());
		results->AddComponent(new Willem::TextComponent(results.get(), "--RESULTS--", font, { 255,0,0 }));

		scene.Add(results);
	}

	{
		auto shotsFired = std::make_shared<Willem::GameObject>("ShotsFired");
		shotsFired->AddComponent(new TransformComponent({ xPos,startY + offsetY,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		shotsFired->AddComponent(new RenderComponent());
		shotsFired->AddComponent(new Willem::TextComponent(shotsFired.get(), "SHOTS FIRED:", font, { 255,255,0 }));

		scene.Add(shotsFired);

		auto shotsFiredNumber = std::make_shared<Willem::GameObject>("ShotsFiredNumber");
		shotsFiredNumber->AddComponent(new TransformComponent({ xPos + offsetX,startY + offsetY,10 }, 1.0f));

		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		shotsFiredNumber->AddComponent(new RenderComponent());
		shotsFiredNumber->AddComponent(new Willem::TextComponent(shotsFiredNumber.get(), std::to_string(AmountOfBulletsFired), font2, { 255,255,0 }));

		scene.Add(shotsFiredNumber);
	}

	{
		auto numberOfHits = std::make_shared<Willem::GameObject>("NumberOfHits");
		numberOfHits->AddComponent(new TransformComponent({ xPos,startY + offsetY * 2,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		numberOfHits->AddComponent(new RenderComponent());
		numberOfHits->AddComponent(new Willem::TextComponent(numberOfHits.get(), "NUMBER OF HITS:", font, { 255,255,0 }));

		scene.Add(numberOfHits);

		auto numberOfHitsNumber = std::make_shared<Willem::GameObject>("NumberOfHitsNumber");
		numberOfHitsNumber->AddComponent(new TransformComponent({ xPos + offsetX,startY + offsetY * 2,10 }, 1.0f));

		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		numberOfHitsNumber->AddComponent(new RenderComponent());
		numberOfHitsNumber->AddComponent(new Willem::TextComponent(numberOfHitsNumber.get(), std::to_string(AmountOfBulletsHit), font2, { 255,255,0 }));

		scene.Add(numberOfHitsNumber);
	}

	{
		auto hitMissRatio = std::make_shared<Willem::GameObject>("HitMissRatio");
		hitMissRatio->AddComponent(new TransformComponent({ xPos,startY + offsetY * 3,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		hitMissRatio->AddComponent(new RenderComponent());
		hitMissRatio->AddComponent(new Willem::TextComponent(hitMissRatio.get(), "HIT MISS RATIO:", font, { 255,255,0 }));

		scene.Add(hitMissRatio);

		auto hitMissRatioNumber = std::make_shared<Willem::GameObject>("HitMissRatioNumber");
		hitMissRatioNumber->AddComponent(new TransformComponent({ xPos + offsetX,startY + offsetY * 3,10 }, 1.0f));

		std::string hitMissRat;

		if (AmountOfBulletsFired > 0)
			hitMissRat = "%" + std::to_string(int(AmountOfBulletsHit * 100 / AmountOfBulletsFired));
		else
			hitMissRat = "%" + std::to_string(100);


		auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		hitMissRatioNumber->AddComponent(new RenderComponent());
		hitMissRatioNumber->AddComponent(new Willem::TextComponent(hitMissRatioNumber.get(), hitMissRat, font2, { 255,255,0 }));

		scene.Add(hitMissRatioNumber);
	}

	
}
void Minigin::LoadCoOpScene() const
{
	ServiceLocator::GetSoundSystem().QueueSound("StartGame", false, 0.1f);

	auto& scene = SceneManager::GetInstance().CreateScene("CoOpScene", (int)GameMode::CoOp);
	LoadHUD(scene);

	{
		auto player = std::make_shared<Willem::GameObject>("Player1");

		SDL_Rect playerSrcRect = { 109,1,16,16 };

		const float offsetFromBottom = 50.0f;
		const Willem::Vector2 playerHalfSize = { playerSrcRect.w / 2.0f,playerSrcRect.h / 2.0f };
		const Willem::Vector3 playerPos = { m_WindowSurface->w / 4 + playerHalfSize.x, m_WindowSurface->h - float(playerSrcRect.h * GAMESCALE) - offsetFromBottom,1 };

		player->AddComponent(new ControlComponent(playerPos));

		player->AddComponent(new RenderComponent(playerSrcRect));
		player->SetTexture("Galaga2.png");

		player->AddComponent(new TransformComponent(playerPos, float(GAMESCALE)));
		player->AddComponent(new ShootComponent());
		player->AddComponent(new HealthComponent(3));
		player->AddComponent(new ScoreComponent(0));

		//player->AddWatcher(new LivesObserver());								<<< UNCOMMENT
		player->AddWatcher(new ScoreObserver());
		player->AddWatcher(new GameOverObserver());
		player->AddTag("Player");
		player->AddTag("Player1");
		CollisionManager::GetInstance().AddCollider(player);
		scene.AddPlayer(player);
	}
	{
		auto player = std::make_shared<Willem::GameObject>("Player2");

		SDL_Rect playerSrcRect = { 109,1,16,16 };

		const float offsetFromBottom = 50.0f;
		const Willem::Vector2 playerHalfSize = { playerSrcRect.w / 2.0f,playerSrcRect.h / 2.0f };
		const Willem::Vector3 playerPos = { m_WindowSurface->w * 0.75f + playerHalfSize.x, m_WindowSurface->h - float(playerSrcRect.h * GAMESCALE) - offsetFromBottom,1 };

		player->AddComponent(new ControlComponent(playerPos));

		player->AddComponent(new RenderComponent(playerSrcRect));
		player->SetTexture("Galaga2.png");

		player->AddComponent(new TransformComponent(playerPos, float(GAMESCALE)));
		player->AddComponent(new ShootComponent());
		player->AddComponent(new HealthComponent(3));
		player->AddComponent(new ScoreComponent(0));							

		//player->AddWatcher(new LivesObserver());								<<< UNCOMMENT
		player->AddWatcher(new ScoreObserver());				
		player->AddTag("Player");
		player->AddTag("Player2");
		CollisionManager::GetInstance().AddCollider(player);
		scene.AddPlayer(player);
	}
	
	float player2HUDOffset = 100.0f;
	{
		auto twoUpHUD = std::make_shared<Willem::GameObject>("TwoUpHUD");
		twoUpHUD->AddComponent(new TransformComponent({ float(m_WindowSurface->w - player2HUDOffset),5.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		twoUpHUD->AddComponent(new RenderComponent());
		twoUpHUD->AddComponent(new Willem::TextComponent(twoUpHUD.get(), "2UP", font, { 255,0,0 }));

		scene.Add(twoUpHUD);
	}

	{
		auto scoreHUD2 = std::make_shared<Willem::GameObject>("ScoreHUD2");
		scoreHUD2->AddComponent(new TransformComponent({ float(m_WindowSurface->w - player2HUDOffset),25.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		scoreHUD2->AddComponent(new RenderComponent());
		scoreHUD2->AddComponent(new Willem::TextComponent(scoreHUD2.get(), "0", font, { 255,255,255 }));

		scene.Add(scoreHUD2);
	}
	{
		auto lifeCounterOne = std::make_shared<Willem::GameObject>("LifeCounter4");
		const SDL_Rect srcRect = { 109,1,16,16 };
		const float offset = 5.0f;
		lifeCounterOne->AddComponent(new TransformComponent({ m_WindowSurface->w - offset- srcRect.w * GAMESCALE,float(m_WindowSurface->h - srcRect.h * GAMESCALE - offset),10 }, GAMESCALE));
		lifeCounterOne->AddComponent(new RenderComponent(srcRect));
		lifeCounterOne->SetTexture("Galaga2.png");
		scene.Add(lifeCounterOne);
	}
	{
		auto lifeCounterTwo = std::make_shared<Willem::GameObject>("LifeCounter3");
		const SDL_Rect srcRect = { 109,1,16,16 };
		const float offset = 5.0f;
		lifeCounterTwo->AddComponent(new TransformComponent({ m_WindowSurface->w - offset - (srcRect.w * GAMESCALE) * 2,float(m_WindowSurface->h - srcRect.h * GAMESCALE - offset),10 }, GAMESCALE));
		lifeCounterTwo->AddComponent(new RenderComponent(srcRect));
		lifeCounterTwo->SetTexture("Galaga2.png");
		scene.Add(lifeCounterTwo);
	}
}

void Minigin::LoadVersusScene() const
{
	ServiceLocator::GetSoundSystem().QueueSound("StartGame", false, 0.1f);

	auto& scene = SceneManager::GetInstance().CreateScene("VersusScene", (int)GameMode::Versus);
	LoadHUD(scene);

	auto player = std::make_shared<Willem::GameObject>("Player1");

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
	player->AddComponent(new ScoreComponent(0));

	player->AddWatcher(new ScoreObserver());
	player->AddWatcher(new GameOverObserver());
	player->AddTag("Player");
	player->AddTag("Player1");
	CollisionManager::GetInstance().AddCollider(player);
	scene.AddPlayer(player);
}

void Minigin::LoadHUD(Willem::Scene& scene) const
{
	{
		auto backgroundHUD = std::make_shared<Willem::GameObject>("BackgroundHUD1");
		backgroundHUD->AddComponent(new TransformComponent({ 0,0,0 },0.35f));

		const SDL_Rect srcRect = { 0,0,2048,2048 };
		backgroundHUD->AddComponent(new BackgroundComponent());
		backgroundHUD->AddComponent(new RenderComponent(srcRect));
		backgroundHUD->SetTexture("Background.png");
		scene.Add(backgroundHUD);
	}
	{
		auto backgroundHUD = std::make_shared<Willem::GameObject>("BackgroundHUD2");
		backgroundHUD->AddComponent(new TransformComponent({ 0,-2048 * 0.35f,0 }, 0.35f));

		const SDL_Rect srcRect = { 0,0,2048,2048 };
		backgroundHUD->AddComponent(new BackgroundComponent());
		backgroundHUD->AddComponent(new RenderComponent(srcRect));
		backgroundHUD->SetTexture("Background.png");
		scene.Add(backgroundHUD);
	}
	{
		auto oneUpHUD = std::make_shared<Willem::GameObject>("OneUpHUD");
		oneUpHUD->AddComponent(new TransformComponent({ 20,5.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		oneUpHUD->AddComponent(new RenderComponent());
		oneUpHUD->AddComponent(new Willem::TextComponent(oneUpHUD.get(), "1UP", font, { 255,0,0 }));

		scene.Add(oneUpHUD);
	}

	{
		auto scoreHUD = std::make_shared<Willem::GameObject>("ScoreHUD");
		scoreHUD->AddComponent(new TransformComponent({ 20,25.0f,10 }, 1.0f));

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
		scoreHUD->AddComponent(new RenderComponent());
		scoreHUD->AddComponent(new Willem::TextComponent(scoreHUD.get(), "0", font, { 255,255,255 }));

		scene.Add(scoreHUD);
	}

	{
		auto lifeCounterOne = std::make_shared<Willem::GameObject>("LifeCounter1");
		const SDL_Rect srcRect = { 109,1,16,16 };
		const float offset = 5.0f;
		lifeCounterOne->AddComponent(new TransformComponent({ offset,float(m_WindowSurface->h - srcRect.h * GAMESCALE - offset),10 }, GAMESCALE));
		lifeCounterOne->AddComponent(new RenderComponent(srcRect));
		lifeCounterOne->SetTexture("Galaga2.png");
		scene.Add(lifeCounterOne);
	}
	{
		auto lifeCounterTwo = std::make_shared<Willem::GameObject>("LifeCounter2");
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
	//LoadGameOverScreen(GameMode::SinglePlayer);

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

	std::thread audioThread(&SoundSystem::Update, &ServiceLocator::GetSoundSystem());

	while (doContinue)
	{

		const auto currentTime = high_resolution_clock::now();
		float deltaTime = duration<float>(currentTime - lastTime).count();

		if (deltaTime > m_DeltaTimeCap)
			deltaTime = m_DeltaTimeCap;

		lastTime = currentTime;
		lag += deltaTime;

		doContinue = input.ProcessInput();

		while (lag >= MsPerUpdate)
		{
			sceneManager.Update(deltaTime);
			enemyManager.Update(deltaTime);
			collisionManager.Update(deltaTime);
			explosionManager.Update(deltaTime);
			
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

void Minigin::ClearScreen()
{
	EnemyManager::GetInstance().Reset();
	CollisionManager::GetInstance().ClearColliders();
	ExplosionManager::GetInstance().Clear();
	SceneManager::GetInstance().RemoveCurrentScene();

	
}
