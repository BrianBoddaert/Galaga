#pragma once
#include "Observer.h"

class GameObject;

class ScoreObserver :
    public Willem::Observer
{
public:
    void OnNotify(const Willem::GameObject* actor, const EVENT& event) override;

private:
    void Unlock(const Willem::GameObject* actor, const EVENT& event);
};

