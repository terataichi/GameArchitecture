#include "DxLib.h"
#include "GameCommon.h"
#include "SceneManager.h"
#include "Quaternion.h"
#include "AsoUtility.h"
#include "Transform.h"
#include "SpriteAnimator.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "Turret.h"
#include "TurretShot.h"
Turret::Turret(SceneManager* manager,
	Transform* transformPlayer,
	Transform* transformParent,
	VECTOR localPos,
	VECTOR localAddAxis)
{

	mSceneManager = manager;
	mTransformPlayer = transformPlayer;

	mTransformParent = transformParent;
	mLocalPos = localPos;
	mLocalAddAxis = localAddAxis;

	ResourceManager* rem = mSceneManager->GetResource();

	mTransformStand.SetModel(rem->LoadModelDuplicate(ResourceManager::SRC::TURRET_STAND));
	mTransformGun.SetModel(rem->LoadModelDuplicate(ResourceManager::SRC::TURRET_GUN));

	mExplosion = new SpriteAnimator(
		mSceneManager, ResourceManager::SRC::SHIP_EXPLOSION, 120, 8.0f);

	mStepDamaged = -1;

	// 砲身ローカル回転
	mLocalAddAxisGun = { 0.0f, 0.0f, 0.0f };

	ChangeState(STATE::ATTACK);

}

void Turret::Init(void)
{

	Quaternion localRot;
	Quaternion axis;

	// モデル制御の基本情報(砲台)
	mTransformStand.scl = { SCALE, SCALE, SCALE };
	mTransformStand.quaRot = Quaternion();

	// ローカル回転
	mTransformStand.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(&mTransformStand, { 0, 0, 0 });



	// モデル制御の基本情報(砲身)
	mTransformGun.scl = { SCALE, SCALE, SCALE };
	mTransformGun.quaRot = Quaternion();

	// ローカル回転
	mTransformGun.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(&mTransformGun, { 0, 0, 0 });

	// 耐久力
	mHp = 2;

	// 砲台ローカル稼働量(deg)
	mAnglePowStand = ANGLE_POW_STAND;

	// 砲身ローカル稼働量(deg)
	mAnglePowGun = ANGLE_POW_GUN;

	shotTime_ = SHOT_TIME;
}

void Turret::Update(void)
{

	switch (mState)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
	{

		float deg;

		// 砲台ローカル回転
		mLocalAddAxisStand.y += mAnglePowStand * mSceneManager->GetDeltaTime();
		if (AsoUtility::Rad2DegF(mLocalAddAxisStand.y) <= ANGLE_Y_MIN_STAND || AsoUtility::Rad2DegF(mLocalAddAxisStand.y) >= ANGLE_Y_MAX_STAND)
		{
			mAnglePowStand *= -1;
		}
		// 砲台
		SyncParent(&mTransformStand, mLocalAddAxisStand);

		// 砲台ローカル回転
		mLocalAddAxisGun.x += mAnglePowGun * mSceneManager->GetDeltaTime();
		if (AsoUtility::Rad2DegF(mLocalAddAxisGun.x) <= ANGLE_X_MIN_GUN || AsoUtility::Rad2DegF(mLocalAddAxisGun.x) >= ANGLE_X_MAX_GUN)
		{
			mAnglePowGun *= -1;
		}
		// 砲身
		Quaternion stand = Quaternion::Euler(mLocalAddAxisStand);
		Quaternion gun = Quaternion::Euler(mLocalAddAxisGun);
		Quaternion mix = stand.Mult(gun);

		SyncParent(&mTransformGun, mix.ToEuler());

		// 被ダメ判定
		if (mStepDamaged > 0.0f)
		{
			mStepDamaged -= mSceneManager->GetDeltaTime();
		}

		shotTime_ -= mSceneManager->GetDeltaTime();
		if (shotTime_ <= 0.0f)
		{
			shotTime_ = SHOT_TIME;

			TurretShot* shot = new TurretShot(mSceneManager, &mTransformGun);

			VECTOR localPos = Quaternion::PosAxis(
				mTransformGun.quaRot, { 0.0f,2.0f,3.0f }
			);
			VECTOR pos = VAdd(mTransformGun.pos, VScale(localPos, SCALE));

			VECTOR dir = mTransformGun.GetForward();

			shot->Create(pos, dir);

			shot_.emplace_back(std::move(shot));
		}
	}
		break;
	case Turret::STATE::DESTROY:

		// 弾の更新
		mExplosion->Update();
		break;
	}

	for (auto& s : shot_)
	{
		s->Update();
	}
}

void Turret::Draw(void)
{

	switch (mState)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:

		if (mStepDamaged > 0.0f)
		{
			MV1SetMaterialDifColor(mTransformStand.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
			MV1SetMaterialDifColor(mTransformGun.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
		}
		else
		{
			MV1SetMaterialDifColor(mTransformStand.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
			MV1SetMaterialDifColor(mTransformGun.modelId, 0, GetColorF(0.2f, 0.4f, 0.3f, 1.0f));
		}

		MV1DrawModel(mTransformStand.modelId);
		MV1DrawModel(mTransformGun.modelId);

		if (IS_DEBUG)
		{
			DrawSphere3D(mTransformStand.pos, COLLISION_RADIUS, 10, 0xff0000, 0xffffff, false);
		}


		break;
	case Turret::STATE::DESTROY:
		mExplosion->Draw();
		break;
	}

	for (auto& s : shot_)
	{
		s->Draw();
	}
}

void Turret::Release(void)
{

	mExplosion->Release();
	delete mExplosion;

}

void Turret::SyncParent(Transform* transform, VECTOR addAxis)
{
	VECTOR localPos;
	transform->quaRot = mTransformParent->quaRot;
	localPos = Quaternion::PosAxis(transform->quaRot, mLocalPos);
	transform->pos = VAdd(mTransformParent->pos, VScale(localPos, SCALE));

	// 子供の回転
	//Quaternion localRot = Quaternion::Euler(mLocalAddAxis);
	//transform->quaRot = transform->quaRot.Mult(localRot);

	// さらに回転(砲台は往復のY回転、砲身はX)
	Quaternion localRot = Quaternion::Euler(mLocalAddAxisStand);
	mTransformStand.quaRot = transform->quaRot.Mult(localRot);

	localRot = Quaternion::Euler(mLocalAddAxisGun);
	mTransformGun.quaRot = transform->quaRot.Mult(localRot);

	transform->Update();
}

bool Turret::IsAlive(void)
{
	return mState == STATE::ATTACK;
}

VECTOR Turret::GetPos(void)
{
	return mTransformStand.pos;
}

void Turret::Damage(void)
{

	mStepDamaged = TIME_DAMAGED_EFFECT;

	mHp -= 1;
	if (mHp <= 0)
	{
		ChangeState(STATE::DESTROY);
	}

}

std::vector<TurretShot*> Turret::GetShot()
{
	return shot_;
}

void Turret::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
	{
		auto mat = mTransformGun.quaRot.ToMatrix();
		auto localPos = VTransform({ 0.0f, 1.2f, 0.0f }, mat);
		auto birthPos = VAdd(mTransformStand.pos, VScale(localPos, SCALE));
		mExplosion->Create(birthPos);
	}
	break;
	}

}
