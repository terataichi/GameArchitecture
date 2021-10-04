#pragma once
#include "SceneBase.h"
class Unit;
class SceneManager;
class Stage;
class RollBall;
class TitleScene : public SceneBase
{

public:

	TitleScene(SceneManager* manager);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;
	void DrawDebug(void);
private:
	Stage* mStage_;
	Unit* mUnit_;
	RollBall* mRollBall_;
};
