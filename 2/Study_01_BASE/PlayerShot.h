#pragma once
#include "ShotBase.h"

class SceneManager;

class PlayerShot :
    public ShotBase
{
public:

    static constexpr float COLLISION_RADIUS = 5.0f;
    PlayerShot(SceneManager* manager, Transform* transform);

    void Update(void)override;

    float GetSpeed(void)override;
    float GetTimeAlive(void)override;
private:
};

