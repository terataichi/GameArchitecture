#pragma once
#include "SceneBase.h"

class SceneManager;
class BossShip;
class SpaceDome;

class EventScene
	:public SceneBase
{
public:
	EventScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	SceneManager* scene;
	BossShip* boss;
	SpaceDome* doom;
};

