#include "Player.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "ParticleGenerator.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "SpriteAnimator.h"
#include "SpeechBalloon.h"
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
	mTransform.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f);
	mTransform.quaRotLocal = Quaternion();
	
	mTransform.pos = AsoUtility::VECTOR_ZERO;

	mTransform.Update();

	mParticleGenerator = new ParticleGenerator(mSceneManager, mTransform.pos, 30);

	mExprosion = new SpriteAnimator(mSceneManager,
		ResourceManager::SRC::SHIP_EXPLOSION,
		20.0f,
		8.0f);
	mState = STATE::Run;

	mSpeechBalloon = new SpeechBalloon(mSceneManager, SpeechBalloon::TYPE::SPEECH, &mTransform);
	mSpeechBalloon->SetText("–°‚¢");
	mSpeechBalloon->SetTime(15.0f);
	mSpeechBalloon->SetRelativePos({ 30.0f,30.0f,0.0f });

}

void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::Non:
		break;
	case Player::STATE::Run:
		RunUpdate();
		break;
	case Player::STATE::Destroy:
		DestroyUpdate();
		break;
	default:
		break;
	}


	mParticleGenerator->Update();
}

void Player::Draw(void)
{


	switch (mState)
	{
	case Player::STATE::Non:
		break;
	case Player::STATE::Run:
		MV1DrawModel(mTransform.modelId);
		mParticleGenerator->Draw();
		DrawSphere3D(mTransform.pos, COLLISION_RADIUS, 10, 0xffffff, 0xffffff, false);
		break;
	case Player::STATE::Destroy:
		mExprosion->Draw();
		break;
	default:
		break;
	}

#ifdef DEBUG

#endif // DEBUG

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

SpeechBalloon* Player::GetSpeech()
{
	return mSpeechBalloon;
}

void Player::SetState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case Player::STATE::Non:
		break;
	case Player::STATE::Run:
		break;
	case Player::STATE::Destroy:
		mExprosion->Create(mTransform.pos);
		break;
	default:
		break;
	}
}

Player::STATE Player::GetState()
{
	return mState;
}

void Player::RunUpdate()
{
	ProcessTurn();
	VECTOR forward = VNorm(mTransform.GetForward());

	VECTOR moveVec = VScale(forward, MOVE_POW);

	mTransform.pos = VAdd(mTransform.pos, moveVec);

	mTransform.Update();

	mParticleGenerator->SetPos(mTransform.pos);
}

void Player::DestroyUpdate()
{
	if (mExprosion->IsEnd())
	{
		return;
	}
	mExprosion->Update();
}
