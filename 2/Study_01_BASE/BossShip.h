#pragma once
#include "Transform.h"
#include "SceneManager.h"
class Player;
class EventShot;

class BossShip
{
public:
	static constexpr float SPEED = 4.0f;
	// 着弾してからのイベント終了時間
	static constexpr float TIME_IVENT = 4.0f;

	enum class STATE
	{
		None,
		Event,
		Battle,
		Destroy,
		End
	};

	class Player;
	BossShip(SceneManager* manager, Player* playerShip);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	SceneManager* mSceneMng;
	Player* mPlayer;

	Transform mTransform;
	EventShot* shot;
	float mTime;

	STATE state_;
};

