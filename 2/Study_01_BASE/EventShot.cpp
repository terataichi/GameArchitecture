#include "EventShot.h"
#include "SpriteAnimator.h"
#include "Camera.h"
#include "SceneManager.h"

EventShot::EventShot(SceneManager* manager, Transform* transform) :
	ShotBase(manager, transform)
{
}

void EventShot::Update(void)
{
	// 着弾時のエフェクト
	mExplosion->Update();

	// 移動処理
	Move();

	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (pos2D.z <= 0 || pos2D.z >= 1)
	{
		CreateExplosion();
	}
	// モデル制御の基本情報更新
	mTransform.Update();
}

float EventShot::GetSpeed(void)
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void)
{
	return 10.0f;
}

