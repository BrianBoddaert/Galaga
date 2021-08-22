#pragma once
#include <Component.h>

class BackgroundComponent final :
	public Willem::Component
{
public:
	BackgroundComponent();
	void Update(float deltaT) override;
private:
	const float m_Speed;
	const float m_Offset;
	Willem::TransformComponent* m_Transform;
};

