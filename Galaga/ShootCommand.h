#pragma once
#include <Command.h>

class ShootCommand final :
    public Willem::Command
{
 public:
	ShootCommand(int controllerIndex) : Willem::Command(controllerIndex) {};
	~ShootCommand() override = default;
	ShootCommand(const ShootCommand&) = delete;
	ShootCommand& operator=(const ShootCommand&) = delete;
	ShootCommand(ShootCommand&&) = delete;
	ShootCommand& operator= (ShootCommand&&) = delete;

	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};

