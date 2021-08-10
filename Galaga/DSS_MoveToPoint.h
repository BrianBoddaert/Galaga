#pragma once
#include "DiveSubState.h"
#include "Structs.h"


class DSS_MoveToPoint final:
    public DiveSubState
{
public:
	DSS_MoveToPoint(Willem::GameObject* go, const Willem::Vector2& pos, const Willem::Vector2& dir, float speedMultiplier = 1.0f) noexcept;

	~DSS_MoveToPoint();
	DSS_MoveToPoint(const DSS_MoveToPoint& other) = delete;
	DSS_MoveToPoint(DSS_MoveToPoint&& other) = delete;
	DSS_MoveToPoint& operator=(const DSS_MoveToPoint& other) = delete;
	DSS_MoveToPoint& operator=(DSS_MoveToPoint&& other) = delete;

	void SetDestinationAndCalculateDirection(const Willem::Vector2& pos);
	void Update(float) override;

protected:
	void Enter() override;
	void Exit()  override;

	Willem::Vector2 m_Direction;
	Willem::Vector2 m_Destination;
	const float m_SpeedMultiplier;


};

