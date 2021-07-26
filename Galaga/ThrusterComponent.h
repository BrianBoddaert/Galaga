#pragma once
#include <Component.h>
class ThrusterComponent :
    public Willem::Component
{
public:
    ThrusterComponent(const Willem::Vector2& dir);
    void Update(float deltaT) override;
private:
    const Willem::Vector2 m_Direction = {};
    static const float m_ProjectileSpeed;
};

