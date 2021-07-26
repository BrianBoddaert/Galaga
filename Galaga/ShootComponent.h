#pragma once
#include <Component.h>
#include "GameObject.h"

class ShootComponent :
    public Willem::Component
{
public:
    void Fire(const Willem::Vector2& dir);
    void Update(float deltaT) override;

private:
    std::vector<std::weak_ptr<Willem::GameObject>> m_Bullets;
};

