#pragma once
#include <Component.h>
#include "GameObject.h"
#include "Structs.h"
class ShootComponent final:
    public Willem::Component
{
public:
    void Fire(const Willem::Vector2& dir);
    void DoubleFire(const Willem::Vector2& dir);
    void Update(float deltaT) override;
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
private:
    std::vector<std::weak_ptr<Willem::GameObject>> m_Bullets;

    float m_DoubleFireInterval = 0.1f;
    float m_DoubleFireCounter = 0.0f;
    bool m_DoubleFireActive = false;
    Willem::Vector2 m_DoubleFireDir = { 0,0 };

    bool m_Enabled = true;
};

