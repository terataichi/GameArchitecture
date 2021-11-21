#pragma once
#include "ShotBase.h"

class SceneManager;

class EventShot :
	public ShotBase
{
public:
	EventShot(SceneManager* manager, Transform* transform);

	void Update(void)override;

	float GetSpeed(void)override;
	float GetTimeAlive(void)override;
private:
};

