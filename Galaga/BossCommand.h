#pragma once
#include <Command.h>

enum class BossMoves
{
	Shoot,
	Dive,
	TractorBeam
};

class BossCommand : public Willem::Command
{
public:
	BossCommand(int controllerIndex) : Willem::Command(controllerIndex) {};
	~BossCommand() override = default;
	BossCommand(const BossCommand&) = delete;
	BossCommand& operator=(const BossCommand&) = delete;
	BossCommand(BossCommand&&) = delete;
	BossCommand& operator= (BossCommand&&) = delete;
	void Execute(const int& parameter = 0) override;
	void Release(const int& parameter = 0) override;
	void Undo() override;
};


