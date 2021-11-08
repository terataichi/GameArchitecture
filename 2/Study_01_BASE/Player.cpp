#include "Player.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "ParticleGenerator.h"

namespace
{
	constexpr float MOVE_POW = 10.0f;
	constexpr float MOVE_ROT_DEG_X = 1.0f;
	constexpr float MOVE_ROT_DEG_Y = 1.0f;
}

Player::Player(SceneManager* manager)
{
	mSceneManager = manager;
}

void Player::Init(void)
{
	mTransform.SetModel(MV1LoadModel("Model/PlayerShip/PlayerShip.mv1"));
	float scale = 10.0;
	mTransform.scl = { scale,scale,scale };
	mTransform.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	mTransform.quaRotLocal = Quaternion();
	
	mTransform.pos = AsoUtility::VECTOR_ZERO;

	mTransform.Update();

	mParticleGenerator = new ParticleGenerator(mSceneManager, mTransform.pos, 30);
}

void Player::Update(void)
{
	ProcessTurn();
	VECTOR forward = VNorm(mTransform.GetForward());

	VECTOR moveVec = VScale(forward, MOVE_POW);

	mTransform.pos = VAdd(mTransform.pos, moveVec);

	mTransform.Update();

	mParticleGenerator->SetPos(mTransform.pos);
	mParticleGenerator->Update();
}

void Player::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
	mParticleGenerator->Draw();
}

void Player::Release(void)
{
	mParticleGenerator->Release();
}

Transform Player::GetTransForm(void)
{
	return mTransform;
}

void Player::ProcessTurn(void)
{
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		Turn(MOVE_ROT_DEG_X, AsoUtility::AXIS_Y);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		Turn(-MOVE_ROT_DEG_X, AsoUtility::AXIS_Y);
	}
	if (CheckHitKey(KEY_INPUT_UP))
	{
		Turn(-MOVE_ROT_DEG_Y, AsoUtility::AXIS_X);
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		Turn(MOVE_ROT_DEG_Y, AsoUtility::AXIS_X);
	}
}

void Player::Turn(float angle, VECTOR axis)
{
	double rad = AsoUtility::Deg2RadD(angle);
	Quaternion tmpQ = Quaternion::AngleAxis(rad, axis);
	mTransform.quaRot = mTransform.quaRot.Mult(tmpQ);
}
