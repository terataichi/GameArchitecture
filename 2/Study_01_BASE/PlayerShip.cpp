#include "PlayerShip.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "Camera.h"

PlayerShip::PlayerShip(SceneManager* scene)
	:mSceneManager(scene)
{
	Init();
}

PlayerShip::~PlayerShip()
{
}

void PlayerShip::Init(void)
{
	mTransform.SetModel(MV1LoadModel("Model/PlayerShip/PlayerShip.mv1"));
	float scl = 10.0f;
	mTransform.scl = { scl ,scl ,scl };
	mTransform.pos = AsoUtility::VECTOR_ZERO;
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = Quaternion();

	mTransform.Update();
}

void PlayerShip::Update(void)
{
	ProcessTurn();
	// プレイヤーの前ぽ方向
	VECTOR  forwatd = mTransform.GetForward();
	// 移動
	mTransform.pos = VAdd(mTransform.pos, VScale(forwatd, SPEED_MOVE));

	// モデル情報などの更新
	mTransform.Update();
}

void PlayerShip::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void PlayerShip::Release(void)
{
}

Transform* PlayerShip::GetTransform(void)
{
	return &mTransform;
}

void PlayerShip::ProcessTurn()
{
	Turn(CheckHitKey(KEY_INPUT_RIGHT), SPEED_ROT_DEG_Y, { 0.0f,1.0f,0.0f });
	Turn(CheckHitKey(KEY_INPUT_LEFT), -SPEED_ROT_DEG_Y, { 0.0f,1.0f,0.0f });
	Turn(CheckHitKey(KEY_INPUT_UP), SPEED_ROT_DEG_X, { 1.0f,0.0f,0.0f });
	Turn(CheckHitKey(KEY_INPUT_DOWN), -SPEED_ROT_DEG_X, { 1.0f,0.0f,0.0f });
}

void PlayerShip::Turn(bool checkKey, double deg, VECTOR axis)
{
	if (checkKey)
	{
		double rad = AsoUtility::Deg2RadD(deg);
		Quaternion tmpQ = Quaternion::AngleAxis(rad, axis);
		mTransform.quaRot = mTransform.quaRot.Mult(tmpQ);
	}
}
