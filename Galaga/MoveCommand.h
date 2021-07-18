#pragma once
#include "Command.h"

enum class MoveInputDirections
{
	Up,
	Right,
	Down,
	Left,
	Count
};

class MoveCommand : public Willem::Command
{
public:
	MoveCommand(int controllerIndex) : Willem::Command(controllerIndex) {};
	~MoveCommand() override = default;
	MoveCommand(const MoveCommand&) = delete;
	MoveCommand& operator=(const MoveCommand&) = delete;
	MoveCommand(MoveCommand&&) = delete;
	MoveCommand& operator= (MoveCommand&&) = delete;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};


