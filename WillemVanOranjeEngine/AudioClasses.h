#pragma once
#include "Singleton.h"
#include <SDL_mixer.h>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <queue>

namespace Willem
{
	enum class EffectId
	{
		Jump,
		Fall,
		Lift,
		Victory
	};

	enum class MusicId
	{
		Ambient
	};

	struct StoredSound
	{
		int soundId;
		float volume;
		bool music;
	};

	// Base SoundSystem class

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual void QueueSound(const EffectId&, float) = 0;
		virtual void QueueSound(const MusicId&, float) = 0;
		virtual void AddSoundToLibrary(const EffectId&, const std::string&) = 0;
		virtual void AddSoundToLibrary(const MusicId&, const std::string&) = 0;
		virtual void Update() = 0;
		virtual void TogglePause() = 0;
	protected:
		virtual void Play(const EffectId&, float) = 0;
		virtual void Play(const MusicId&, float) = 0;
	public:

		SoundSystem() = default;
		SoundSystem(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator= (SoundSystem&&) = delete;

	};

	// Child SoundSystem classes
	class SdlSoundSystem final : public SoundSystem
	{
	public:
		SdlSoundSystem()
		{
			m_Playing.store(true);
		}

		~SdlSoundSystem()
		{
			m_Playing.store(false);
			m_QueueActive.notify_one();

			std::cout << " ";
			for (auto const& x : m_SoundLibrary)
			{
				Mix_FreeChunk(m_SoundLibrary.at(x.first));
			}

			for (auto const& x : m_MusicLibrary)
			{
				Mix_FreeMusic(m_MusicLibrary.at(x.first));
			}
		}

		SdlSoundSystem(const SdlSoundSystem& other) = delete;
		SdlSoundSystem(SdlSoundSystem&& other) = delete;
		SdlSoundSystem& operator=(const SdlSoundSystem& other) = delete;
		SdlSoundSystem& operator=(SdlSoundSystem&& other) = delete;

		void QueueSound(const EffectId& key, float volume = 1.0f) override
		{

			StoredSound sound{ int(key),volume,false };
			std::lock_guard<std::mutex> mLock{ m_Mutex };
			m_SoundQueue.push(sound);
			m_QueueActive.notify_one();
		}

		void QueueSound(const MusicId& key, float volume = 1.0f) override
		{

			StoredSound sound{ int(key),volume,true };
			std::lock_guard<std::mutex> mLock{ m_Mutex };
			m_SoundQueue.push(sound);
			m_QueueActive.notify_one();
		}

		void AddSoundToLibrary(const EffectId& soundId, const std::string& path) override
		{
			m_SoundLibrary.insert(std::pair<EffectId, Mix_Chunk*>(soundId, Mix_LoadWAV(path.c_str())));
		}

		void AddSoundToLibrary(const MusicId& soundId, const std::string& path) override
		{
			m_MusicLibrary.insert(std::pair<MusicId, Mix_Music*>(soundId, Mix_LoadMUS(path.c_str())));
		}

		void Update() override
		{
			while (m_Playing.load())
			{
				while (!m_SoundQueue.empty())
				{
					std::unique_lock<std::mutex> mLock{ m_Mutex };
					StoredSound ps = m_SoundQueue.front();
					m_SoundQueue.pop();

					if (ps.music)
						Play(MusicId(ps.soundId), ps.volume);
					else
						Play(EffectId(ps.soundId), ps.volume);
				}

				std::unique_lock<std::mutex> guard{ m_Mutex };
				m_QueueActive.wait(guard);
			}

		}

		void TogglePause() override
		{
			if (Mix_PausedMusic())
				Start();
			else
				Stop();
		}

	protected:
		void Play(const EffectId& key, float volume = 1.0f) override
		{
			if (m_SoundLibrary.find(key) != m_SoundLibrary.end())
			{
				auto sound = m_SoundLibrary.at(key);

				Mix_Volume(-1, int(volume * 128.0f));
				Mix_PlayChannel(-1, sound, 0);
			}
			else
			{
				std::cout << "SoundId not found when attempting to play sound" << std::endl;
			}
		};
		void Play(const MusicId& key, float volume = 1.0f) override
		{
			if (m_MusicLibrary.find(key) != m_MusicLibrary.end())
			{
				auto sound = m_MusicLibrary.at(key);

				Mix_VolumeMusic(int(volume * 128.0f));
				Mix_PlayMusic(sound, 0);
			}
			else
			{
				std::cout << "MusicId not found when attempting to play music" << std::endl;
			}
		};
		void Start()
		{
			Mix_Resume(-1);
			Mix_ResumeMusic();
		}

		void Stop()
		{
			Mix_Pause(-1);
			Mix_PauseMusic();
		}
	private:
		std::atomic_bool m_Playing;
		std::mutex m_Mutex;
		std::condition_variable m_QueueActive{};
		std::queue<StoredSound> m_SoundQueue{};
		std::map<MusicId, Mix_Music*> m_MusicLibrary;
		std::map<EffectId, Mix_Chunk*> m_SoundLibrary;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		~NullSoundSystem() override = default;
		NullSoundSystem(const NullSoundSystem&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&) = delete;
		NullSoundSystem(NullSoundSystem&&) = delete;
		NullSoundSystem& operator= (NullSoundSystem&&) = delete;
		void QueueSound(const EffectId&, float) override {};
		void QueueSound(const MusicId&, float) override {};
		void Play(const EffectId&, float ) override {};
		void Play(const MusicId&, float) override {};
		void Update() override {};
		void AddSoundToLibrary(const EffectId&, const std::string&) override {};
		void AddSoundToLibrary(const MusicId&, const std::string&) override {};
		void TogglePause() override {};
	};

	class LoggingSoundSystem final : public SoundSystem
	{
	public:
		LoggingSoundSystem(SoundSystem* ss) :m_pRealSoundSystem(ss) {}

		void Update() override
		{
			m_pRealSoundSystem->Update();
		}

		void QueueSound(const EffectId& soundId, float volume = 1.0f) override
		{
			if (!m_Muted)
			{
				m_pRealSoundSystem->QueueSound(soundId, volume);
				//std::cout << "Playing " << std::to_string((int)soundId) << " at volume " << volume << std::endl;
			}
			//else
			//{
			//	//std::cout << "Attempting to play " << std::to_string((int)soundId) << " but the sound is muted or the ID is invalid" << std::endl;
			//}
		}

		void QueueSound(const MusicId& soundId, float volume = 1.0f) override
		{
			if (!m_Muted)
			{
				m_pRealSoundSystem->QueueSound(soundId, volume);
				//std::cout << "Playing " << std::to_string((int)soundId) << " at volume " << volume << std::endl;
			}
			//else
			//{
			//	//std::cout << "Attempting to play " << std::to_string((int)soundId) << " but the sound is muted or the ID is invalid" << std::endl;
			//}
		}

		void AddSoundToLibrary(const EffectId& soundId, const std::string& path) override
		{
			m_pRealSoundSystem->AddSoundToLibrary(soundId, path);
		}

		void AddSoundToLibrary(const MusicId& soundId, const std::string& path) override
		{
			m_pRealSoundSystem->AddSoundToLibrary(soundId, path);
		}

		~LoggingSoundSystem() 
		{
			delete m_pRealSoundSystem; 
		};

		LoggingSoundSystem(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem&) = delete;
		LoggingSoundSystem(LoggingSoundSystem&&) = delete;
		LoggingSoundSystem& operator= (LoggingSoundSystem&&) = delete;

		//void ToggleMute() { m_Muted = !m_Muted; };
		void TogglePause() override
		{
			m_pRealSoundSystem->TogglePause();
		};

	private:
		SoundSystem* m_pRealSoundSystem;
		bool m_Muted{};

	protected:
		void Play(const EffectId&, float) override {}
		void Play(const MusicId&, float) override {};

	};

	// Service locator
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem();
		static void SetSoundSystem(SoundSystem* ss);

		~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;
		ServiceLocator& operator= (ServiceLocator&&) = delete;

		static void CleanUp();

	private:
		static SoundSystem* m_pSsInstance;
		static NullSoundSystem m_NullSs;
	};


}
