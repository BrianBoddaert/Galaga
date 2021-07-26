#pragma once
#include "AlienState.h"

class FormationState final :
	public AlienState
{
public:
	FormationState(Willem::GameObject* go) noexcept;
	~FormationState();
	FormationState(const FormationState& other) = delete;
	FormationState(FormationState&& other) = delete;
	FormationState& operator=(const FormationState& other) = delete;
	FormationState& operator=(FormationState&& other) = delete;

	void Update(float deltaT) override;
	AlienState* GetFollowUpState() const override;

protected:
	void Enter() override;
	void Exit() override;
};

