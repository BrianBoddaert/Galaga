#pragma once
#include "AlienState.h"
#include "Structs.h"

class GameObject;

enum class DiveStages
{
	Initialize,
	FlyDownDiagonally,
	FlyInCircle,
	ReturnToFormation
};

class SpawnDiveState final :
    public AlienState
{
public:
	SpawnDiveState(Willem::GameObject* go) noexcept;
	~SpawnDiveState();
	SpawnDiveState(const SpawnDiveState& other) = delete;
	SpawnDiveState(SpawnDiveState&& other) = delete;
	SpawnDiveState& operator=(const SpawnDiveState& other) = delete;
	SpawnDiveState& operator=(SpawnDiveState&& other) = delete;

	void Update(float deltaT) override;
	AlienState* GetFollowUpState() const override;

protected:
	void Enter() override;
	void Exit() override;

	float DirectionToLocalOrientationAngle(const Willem::Vector2& dir);

	DiveStages m_Stage;

	const float m_Speed;
	const float m_RotationSpeed;

	// Turning in a circular motion 
	float m_CircularTurnRadians;
	Willem::Vector2 m_CircularTurnCenter;

	//Go to Formation
	Willem::Vector2 m_DestinationPosition;
	Willem::Vector2 m_DestinationDirection;
};

