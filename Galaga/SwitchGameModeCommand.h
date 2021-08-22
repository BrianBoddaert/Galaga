#pragma once
#include "Command.h"

class SwitchGameModeCommand final : public Willem::Command
{
public:
	SwitchGameModeCommand(int controllerIndex) : Willem::Command(controllerIndex) {};
	~SwitchGameModeCommand() override = default;
	SwitchGameModeCommand(const SwitchGameModeCommand&) = delete;
	SwitchGameModeCommand& operator=(const SwitchGameModeCommand&) = delete;
	SwitchGameModeCommand(SwitchGameModeCommand&&) = delete;
	SwitchGameModeCommand& operator= (SwitchGameModeCommand&&) = delete;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};


