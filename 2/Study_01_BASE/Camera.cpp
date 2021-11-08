#include "DxLib.h"
#include "GameCommon.h"
#include "Camera.h"
#include "AsoUtility.h"
#include "Transform.h"
#include "SceneManager.h"

Camera::Camera(SceneManager* manager)
{
	mSceneManager = manager;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	ChangeMode(MODE::FREE);
	SetDefault();

	
}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = VAdd(mPos, RELATIVE_TARGET_POS);
	mCameraUp = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、この傾いた状態を傾き無しとする
	// mQuaRotは回転計算用で、
	// あくまで軸となるのは、カメラ座標と注視点とする
	mQuaRot = Quaternion();

}

void Camera::Update()
{



}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(30.0f, 15000.0f);


	switch (mMode)
	{
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FIXSED_POINT:
		SetBeforeDrawFixsedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;
	default:
		break;
	}

	// カメラの設定
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

}

void Camera::SetBeforeDrawFree(void)
{
	// カメラの方向移動と回転(回転移動)

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	auto move = [&](bool check ,VECTOR dir)
	{
		if (check)
		{
			moveDir = dir;
		}
	};
	move(CheckHitKey(KEY_INPUT_W), AsoUtility::DIR_F);
	move(CheckHitKey(KEY_INPUT_A), AsoUtility::DIR_L);
	move(CheckHitKey(KEY_INPUT_S), AsoUtility::DIR_B);
	move(CheckHitKey(KEY_INPUT_D), AsoUtility::DIR_R);


	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	auto  rotate = [&](bool check, VECTOR dir)
	{
		if (check)
		{
			axisDeg = VAdd(axisDeg,dir);
		}
	};
	rotate(CheckHitKey(KEY_INPUT_UP), { -1.0f,0.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_DOWN), { 1.0f,0.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_LEFT), { 0.0f,-1.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_RIGHT), { 0.0f,1.0f,0.0f });

	// 回転
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		// ラジアン変換
		VECTOR axisRad;
		axisRad.x = AsoUtility::Deg2RadF(axisDeg.x);
		axisRad.y = AsoUtility::Deg2RadF(axisDeg.y);
		axisRad.z = AsoUtility::Deg2RadF(axisDeg.z);

		// クオータニオン更新
		Quaternion tmpAxis = Quaternion::Euler(axisRad);
		mQuaRot = mQuaRot.Mult(tmpAxis);

		// 注視点更新
		VECTOR tmpPos = mQuaRot.PosAxis(RELATIVE_TARGET_POS);
		mTargetPos = VAdd(mPos, tmpPos);

		// カメラの上方向
		mCameraUp = mQuaRot.GetUp();
	}

	// 移動
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 30.0f;


		MATRIX mat = mQuaRot.ToMatrix();
		// 移動させたい方向(ベクトル)を求めたい
		VECTOR dir = VNorm(VTransform(moveDir, mat));

		VECTOR movePower = VScale(dir, speed);

		mPos = VAdd(mPos, movePower);
		mTargetPos = VAdd(mTargetPos, movePower);
	}
}

void Camera::SetBeforeDrawFixsedPoint(void)
{
}

void Camera::SetBeforeDrawFollow(void)
{
	if (mShipTransform == nullptr)
	{
		return;
	}

	// プレイヤー座標を取得
	VECTOR shiPos = mShipTransform->pos;
	// プレイヤーの向き取得
	Quaternion shipRot = mShipTransform->quaRot;

	// カメラ位置
	// -------------------------------------------------------------------
	// mPos = VAdd(shiPos, RELATIVE_CAMERA_FOLLOW);
	// -> プレイヤーが正面を向いている時しか対応できない
	//	  相対座標に向かってプレイヤーの開店を加える必要がある
	VECTOR soutaiCameraPos;
	soutaiCameraPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW);

	mPos = VAdd(shiPos, soutaiCameraPos);
	// -------------------------------------------------------------------


	// 注視点
	// -------------------------------------------------------------------
	// mTargetPos
	VECTOR soutaiTargetPos;
	soutaiTargetPos = shipRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, soutaiTargetPos);
	// -------------------------------------------------------------------

	// カメラの上方向
	// -------------------------------------------------------------------
	// mCameraUp
	mCameraUp = shipRot.GetUp();
	// -------------------------------------------------------------------

	// 時機とカメラ位置との相対座標
	RELATIVE_CAMERA_FOLLOW;

	// カメラ位置とRELATIVE_TARGET_POS相対座標
	RELATIVE_TARGET_POS;

}

void Camera::SetBeforeDrawFollowSpring(void)
{
	if (mShipTransform == nullptr)
	{
		return;
	}

	// (定数予定)
	float POW_SPRING = 24.0f;
	float dampening = 2.0 * sqrt(POW_SPRING);

	float delta = mSceneManager->GetDeltaTime();

	// プレイヤー座標を取得
	VECTOR shiPos = mShipTransform->pos;
	// プレイヤーの向き取得
	Quaternion shipRot = mShipTransform->quaRot;

	// カメラの相対位置
	VECTOR soutaiCameraPos;
	soutaiCameraPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW_SPRIBNG);

	// 理想位置
	VECTOR idealPos = VAdd(shiPos, soutaiCameraPos);

	// 現在位置と理想位置の差
	VECTOR diff = VSub(mPos, idealPos);

	// ばねの力
	VECTOR force = VSub(VScale(diff, -POW_SPRING), VScale(mVelosity, dampening));

	// カメラ速度を更新
	mVelosity = VAdd(mVelosity,VScale(force, delta));
	mPos = VAdd(mPos, VScale(mVelosity, delta));

	// mTargetPos
	VECTOR soutaiTargetPos;
	soutaiTargetPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW_SPRIBNG);
	mTargetPos = VAdd(mPos, soutaiTargetPos);

	mCameraUp = shipRot.GetUp();
}

void Camera::Draw()
{
}

void Camera::DrawDebug(void)
{
}

void Camera::DrawUI(void)
{

	// 操作説明
	int x = 200;
	int y = 10;
	int addY = 40;
	DrawString(
		SCREEN_SIZE_X - x, y, "　【操作】", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "　　　　旋回：↑↓←→", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "　　　　加速：Ｂ", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "　　　　発弾：Ｎ", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "　シーン遷移：Ｓｐａｃｅ", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "　1500m以上離れると", 0xffffff);
	y += 20;
	DrawString(
		SCREEN_SIZE_X - x, y, "　ゲームオーバー", 0xffffff);

}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void)
{
	return mPos;
}

VECTOR Camera::GetTargetPos(void)
{
	return mTargetPos;
}

VECTOR Camera::GetDir(void)
{
	return VNorm(VSub(mTargetPos, mPos));
}

void Camera::ChangeMode(MODE mode)
{
	SetDefault();
	mMode = mode;
}

void Camera::SetShip(Transform* trans)
{
	mShipTransform = trans;
}
