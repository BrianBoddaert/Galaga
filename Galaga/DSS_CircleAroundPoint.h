#pragma once
#include "DiveSubState.h"
#include "Structs.h"


class DSS_CircleAroundPoint final :
	public DiveSubState
{
public:
	DSS_CircleAroundPoint(Willem::GameObject* go, const Willem::Vector2& pos, float rot, float rotlimit, bool clockWise = true, const Willem::Vector2& offset = { 0,50 }) noexcept;

	~DSS_CircleAroundPoint();
	DSS_CircleAroundPoint(const DSS_CircleAroundPoint& other) = delete;
	DSS_CircleAroundPoint(DSS_CircleAroundPoint&& other) = delete;
	DSS_CircleAroundPoint& operator=(const DSS_CircleAroundPoint& other) = delete;
	DSS_CircleAroundPoint& operator=(DSS_CircleAroundPoint&& other) = delete;

	void Update(float) override;

protected:
	void Enter() override;
	void Exit()  override;

	Willem::Vector2 m_Center;
	float m_Offset;
	float m_Rotation;
	const float m_RotationLimit;
	bool m_Clockwise;

};

