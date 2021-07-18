#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "Structs.h"


class ControlComponent :
	public Willem::Component
{
public:
	ControlComponent(const Willem::Vector3& spawn);
	void Initialize();
	void SetMoveInput(const MoveInputDirections& dir, bool on);
	void Update(float) override;

	void Move(bool right,float deltaT);
	const Willem::Vector3& GetSpawnPosition() const { return m_SpawnPosition; }
private:
	const Willem::Vector3 m_SpawnPosition;

	bool m_MoveInputsActive[long(MoveInputDirections::Count)];
	bool m_Moving;
	float m_Speed;
};




