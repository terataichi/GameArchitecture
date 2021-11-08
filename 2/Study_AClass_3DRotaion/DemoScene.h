#pragma once
#include <vector>
#include "SceneBase.h"
class SceneManager;
class Coin;

class DemoScene : public SceneBase
{

public:

	DemoScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	Coin* mCoin;

};
