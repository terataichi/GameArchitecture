#include "GameCommon.h"
#include "AsoUtility.h"
#include "Quaternion.h"
#include "SceneManager.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "SpriteAnimator.h"
#include "ShotBase.h"

ShotBase::ShotBase(SceneManager* manager, Transform* parent)
{

	// 初期化系
	mSceneManager = manager;
	mTransformParent = parent;
	mExplosion = new SpriteAnimator(
		manager, ResourceManager::SRC::SHOT_EXPLOSION, 20.0f, 8.0f);

	mIsAlive = false;

	// もし変動させたければ専用Getterを
	mCollisionRadius = 10.0f;

	// モデル制御の基本情報
	ResourceManager* rem = mSceneManager->GetResource();
	mTransform.modelId = rem->LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	float scale = 0.2f;
	mTransform.scl = { scale, scale, scale };
	mTransform.pos = mTransformParent->pos;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
	mTransform.Update();

}

void ShotBase::Create(VECTOR birthPos, VECTOR dir)
{

	// 再利用可能なようにする

	// 指定方向に弾を飛ばす
	mDir = dir;

	mTransform.pos = birthPos;
	mTransform.quaRot = Quaternion::LookRotation(mDir);

	// 生存系初期化
	mIsAlive = true;
	mStepAlive = GetTimeAlive();

}

void ShotBase::Update(void)
{

	// 着弾時のエフェクト
	mExplosion->Update();

	if (!CheckAlive() || !IsAlive())
	{
		return;
	}

	// 移動処理
	Move();

	// モデル制御の基本情報更新
	mTransform.Update();

}

bool ShotBase::CheckAlive(void)
{

	// 生存時間
	mStepAlive -= mSceneManager->GetDeltaTime();
	if (mStepAlive < 0.0f)
	{
		mIsAlive = false;
	}

	return mIsAlive;

}

void ShotBase::Move(void)
{

	// 移動
	VECTOR velocity = VScale(mDir, GetSpeed());
	mTransform.pos = VAdd(mTransform.pos, velocity);

}

void ShotBase::Draw(void)
{

	mExplosion->Draw();

	if (!mIsAlive)
	{
		return;
	}

	MV1DrawModel(mTransform.modelId);

}

void ShotBase::Release(void)
{
	mExplosion->Release();
	delete mExplosion;
	//MV1DeleteModel(mTransform.modelId);
}

float ShotBase::GetSpeed(void)
{
	return DEFAULT_SPEED;
}

float ShotBase::GetTimeAlive(void)
{
	return DEFAULT_TIME_ALIVE;
}

bool ShotBase::IsAlive(void)
{
	return mIsAlive;
}

void ShotBase::Dead(void)
{
	mIsAlive = false;
}

void ShotBase::CreateExplosion(void)
{
	mIsAlive = false;
	mExplosion->Create(mTransform.pos);
}

VECTOR ShotBase::GetPos(void)
{
	return mTransform.pos;
}

float ShotBase::GetCollisionRadius(void)
{
	return mCollisionRadius;
}
