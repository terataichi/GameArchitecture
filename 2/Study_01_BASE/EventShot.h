#pragma once
#include "ShotBase.h"

class SceneManager;
class Transform;
class EventShot
	:public ShotBase
{
public:
	EventShot(SceneManager* manager,Transform* trans);

	void Update();
private:

};

