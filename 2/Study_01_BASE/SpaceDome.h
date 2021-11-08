#pragma once
#include "Transform.h"

class SceneManager;
class Player;

class SpaceDome
{
public:

	enum class state
	{
		WAIT,
		FOLLOW,
	};

	SpaceDome(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetPlayer(Player* player);

private:
	SceneManager* mSceneManager;

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform mTransform;

	state state_;
	Player* player_;
};

