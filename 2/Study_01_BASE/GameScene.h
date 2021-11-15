#pragma once
#include <vector>
#include "SceneBase.h"
class SceneManager;
class SpaceDome;
class Stage;
class Player;
class RockManager;

class GameScene : public SceneBase
{

public:
	static constexpr float RESTART_TIME = 2.0;

	GameScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	SpaceDome* mSpaceDome;
	Stage* mStage;
	Player* mPlayer;
	RockManager* rockManager;

	float resetTime_;
};
