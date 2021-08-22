#pragma once
#include "Component.h"
#include "MoveCommand.h"
#include "Structs.h"

#include <memory>


class ControlComponent final :
	public Willem::Component
{
public:
	ControlComponent(const Willem::Vector3& spawn);
	void Initialize();
	void SetMoveInput(const MoveInputDirections& dir, bool on);
	void Update(float) override;
	void SetEnabled(bool enabled) { m_Enabled = enabled; }
	void Move(bool right,float deltaT);
	const Willem::Vector3& GetSpawnPosition() const { return m_SpawnPosition; }
	void EnableCaughtInTractorBeam(std::weak_ptr<Willem::GameObject> boss);
	void DisableCaughtInTractorBeam() { m_CaughtInTractorBeam = false; }
private:
	void TractorBeamImprison(float deltaT);
	void TractorBeamRelease(float deltaT);

	const Willem::Vector3 m_SpawnPosition;
	
	bool m_MoveInputsActive[long(MoveInputDirections::Count)];
	bool m_Moving;
	float m_Speed;

	bool m_Enabled;

	bool m_CaughtInTractorBeam;
	bool m_EscapeFromTractorBeam;
	Willem::Vector2 m_CapturerLocation;
	Willem::Vector2 m_CapturerDirection;
	std::weak_ptr<Willem::GameObject> m_pCapturer;

	const float m_TractorBeamSuckSpeed;
};




