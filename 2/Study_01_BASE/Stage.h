#pragma once
#include "Transform.h"

class SceneManager;

class Stage
{
public:

	Stage(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	SceneManager* mSceneManager;

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform mTransform;
};