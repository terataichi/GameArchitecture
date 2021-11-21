#pragma once
#include "Transform.h"

class SceneManager;
class Player;
class EventShot;

class BossShip
{
public:
	static constexpr float SPEED_MOVE = 6.0f;
	static constexpr float EVENT_TIME = 3.0f;

	enum class Boss_State
	{
		Non,
		Event,
		Battle,
		Exp,
		End,
	};
	BossShip(SceneManager* manager, Player* player);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	SceneManager* sceneManager_;
	Player* player_;
	EventShot* eventShot_;

	Transform transform_;

	float stepEvent_;


	Boss_State state_;

	void ChengeState(Boss_State state);
};

