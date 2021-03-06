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

	// モデル制御の基本情報
	Transform mTransform;

	int mModelDungeon;
	int mBossModel;
};