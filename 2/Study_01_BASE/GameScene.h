#pragma once
#include <vector>
#include "SceneBase.h"
class SceneManager;
class SpaseDome;
class Stage;
class PlayerShip;

class GameScene : public SceneBase
{

public:

	GameScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	SpaseDome* mSpaseDome;

	Stage* mStage;

	PlayerShip* mPlayerShip;
};
