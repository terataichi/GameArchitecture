#pragma once
#include"SceneBase.h"

class SceneManager;
class SpaceDome;
class BossShip;
class RockManager;
class Player;


class BattelScene:
	public SceneBase
{
public:
	BattelScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
private:
	Player* player_;

	RockManager* rockManager_;

	SpaceDome* mSpaceDome;
	BossShip* boss_;

	float playerDeadTime_;
};

