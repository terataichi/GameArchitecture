#include "PlayerShot.h"
#include "SpriteAnimator.h"
#include "SceneManager.h"

PlayerShot::PlayerShot(SceneManager* manager, Transform* transform):
	ShotBase(manager, transform)
{
}

void PlayerShot::Update(void)
{
	// 着弾時のエフェクト
	mExplosion->Update();

	if (!IsAlive())
	{
		return;
	}

	// 移動処理
	Move();

	// モデル制御の基本情報更新
	mTransform.Update();
}


float PlayerShot::GetSpeed(void)
{
	return 20.0f;
}

float PlayerShot::GetTimeAlive(void)
{
	return 10.0f;
}
