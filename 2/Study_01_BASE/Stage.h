#pragma once
#include "Transform.h"

class SceneManager;

class Stage
{
public:
	static constexpr float RADIUS = 12000.0f;
	Stage(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelDungeon();
	int GetModelBoss();
private:
	SceneManager* mSceneManager;

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform mTransform;

	int mModelDungeon;
	int mBossModel;
};