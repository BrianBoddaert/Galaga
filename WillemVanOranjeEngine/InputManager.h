#pragma once
#include "Singleton.h"
#include "Command.h"
#include <map>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <Xinput.h>

#include <SDL.h>


#pragma comment(lib, "XInput.lib")

namespace Willem
{
	enum class ControllerButton
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonStart = XINPUT_GAMEPAD_START,
		TriggerLeft = 10,
		TriggerRight = 11,
		LeftThumbStickLeft = 12,
		LeftThumbStickRight = 13,
		LeftThumbStickUp = 14,
		LeftThumbStickDown = 15,
		RightThumbStickLeft = 17,
		RightThumbStickRight = 18,
		RightThumbStickUp = 23,
		RightThumbStickDown = 20,
		Count = 19
		//todo: add the other buttons
	};

	enum class KeyboardButton
	{
		W = SDLK_w,
		A = SDLK_a,
		S = SDLK_s,
		D = SDLK_d,
		P = SDLK_p,
		SPACE = SDLK_SPACE,
		PLUS = SDLK_EQUALS
	};

	using ControllerKey = std::pair<unsigned, ControllerButton>;
	using ControllerCommandsMap = std::map<ControllerKey, std::pair < std::unique_ptr<Command>, int>>;

	struct Controller
	{
		ControllerCommandsMap m_ConsoleCommands;
		XINPUT_STATE m_CurrentState{};
	};

	using KeyboardKey = std::pair<unsigned, KeyboardButton>;
	using KeyboardCommandsMap = std::map < KeyboardKey, std::pair < std::unique_ptr<Command>, int>>;

	struct KeyboardController
	{
		KeyboardCommandsMap m_ConsoleCommands;

	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() = default;

		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		bool ProcessInput();
		void ControllerInput();
		bool KeyboardInput();
		bool IsPressed(const ControllerButton& button, const std::unique_ptr<Controller>& controller) const;
		bool IsPressed(const BYTE& trigger) const;
		bool IsPressed(const SHORT& analogMovement, bool up) const;

		template <typename T>
		void AssignControllerKey(ControllerButton button, int controllerIndex, int parameter = 0)
		{
			ControllerKey key = std::make_pair(unsigned(button), button);
			m_Controllers[controllerIndex]->m_ConsoleCommands.insert(std::make_pair(key, std::make_pair(std::make_unique<T>(controllerIndex), parameter)));
		}

		template <typename T>
		void AssignKeyboardKey(const KeyboardButton& button, int parameter = 0)
		{
			KeyboardKey key = std::make_pair(unsigned(button), button);
			m_KeyboardController->m_ConsoleCommands.insert(std::make_pair(key, std::make_pair(std::make_unique<T>(-1), parameter)));
			// -1 = keyboard controller
		}

		void LockInput(bool lock = true) { m_InputLocked = lock; }
		bool GetInputLocked() const { return m_InputLocked; }
	private:
		bool m_InputLocked;
		std::unique_ptr <KeyboardController> m_KeyboardController;
		const ControllerButton m_ControllerButtons[(long)ControllerButton::Count];

		std::vector<std::unique_ptr<Controller>> m_Controllers;

		const float m_TriggerSensitivity = 0.75f;
		const float m_TriggerRange = 255.f;

		const float m_AnalogSensitivity = 0.75f;
		const float m_AnalogRange = 32767.f;

	};
}