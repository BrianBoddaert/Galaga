#pragma once
#include "DiveSubState.h"
#include "Structs.h"
#include "GameObject.h"
#include <memory>


class DSS_TractorBeam final :
	public DiveSubState
{
public:
	DSS_TractorBeam(Willem::GameObject* go) noexcept;

	~DSS_TractorBeam();
	DSS_TractorBeam(const DSS_TractorBeam& other) = delete;
	DSS_TractorBeam(DSS_TractorBeam&& other) = delete;
	DSS_TractorBeam& operator=(const DSS_TractorBeam& other) = delete;
	DSS_TractorBeam& operator=(DSS_TractorBeam&& other) = delete;

	void Update(float) override;

protected:
	void Enter() override;
	void Exit()  override;

	std::weak_ptr<Willem::GameObject> m_pTractorBeam;
	const float m_TractorBeamYSpriteInterval;
	float m_TractorBeamYSpriteTimer;
	const float m_TractorBeamXSpriteInterval;
	float m_TractorBeamXSpriteTimer;

	const float m_TractorBeamActiveDuration;
	float m_TractorBeamActiveTimer;
	bool m_TractorBeamActive;

	bool m_TractorBeamExpanding;

};

