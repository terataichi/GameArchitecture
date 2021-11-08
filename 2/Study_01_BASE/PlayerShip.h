#pragma once
#include "Transform.h"

class SceneManager;

class PlayerShip
{
public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// 時機の開店量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	PlayerShip(SceneManager* scene);
	~PlayerShip();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	Transform* GetTransform(void);

	// 時機の操作
	void ProcessTurn();
	void Turn(bool checkKey, double deg, VECTOR axis);
private:

	// モデル情報の基本情報
	Transform mTransform;

	SceneManager* mSceneManager;
};

