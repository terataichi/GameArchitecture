#include "Player.h"
#include "SceneManager.h"
#include "ParticleGenerator.h"
#include "SpriteAnimator.h"
#include "AsoUtility.h"
#include "Camera.h"
#include "SpeechBalloon.h"
#include "PlayerShot.h"

namespace
{
	constexpr float MOVE_POW = 10.0f;
	constexpr float MOVE_ROT_DEG_X = 1.0f;
	constexpr float MOVE_ROT_DEG_Y = 1.0f;
	constexpr float SHOT_INTERVAL = 0.3f;
}


Player::Player(SceneManager* manager)
{
	mSceneManager = manager;
	state_ = PLAYER_STATE::NON;
	parGene_ = nullptr;
}

void Player::Init(void)
{
	mTransform.SetModel(MV1LoadModel("Model/PlayerShip/PlayerShip.mv1"));
	float scale = 10.0;
	mTransform.scl = { scale,scale,scale };
	mTransform.quaRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);
	mTransform.quaRotLocal = Quaternion();
	mTransform.pos = AsoUtility::VECTOR_ZERO;
	spriteAnimator_ = new SpriteAnimator(mSceneManager, ResourceManager::SRC::SHIP_EXPLOSION, 20, 16.0f);

	speechBalloon_ = new SpeechBalloon(mSceneManager, SpeechBalloon::TYPE::SPEECH, &mTransform);
	speechBalloon_->SetText("’Ç‚Á‚ÄI");
	speechBalloon_->SetTime(15.0f);
	speechBalloon_->SetRelativePos({ 25.0f,25.0f,25.0f });

	mTransform.Update();

	shotInterval_ = 0.0f;

	ChengeState(PLAYER_STATE::RUN);

	parGene_ = new ParticleGenerator(mSceneManager, mTransform.pos, 10.0f);
	parGene_->Init();
	isAlive_ = true;
}

void Player::Update(void)
{
	shotInterval_ -= mSceneManager->GetDeltaTime();
	if (shotInterval_ <= 0.0f)
	{
		shotInterval_ = 0.0f;
	}
	switch (state_)
	{
	case Player::PLAYER_STATE::NON:
		break;
	case Player::PLAYER_STATE::RUN:
		ProcessTurn();
		VECTOR forward = VNorm(mTransform.GetForward());

		VECTOR moveVec = VScale(forward, MOVE_POW);

		mTransform.pos = VAdd(mTransform.pos, moveVec);

		mTransform.Update();
		parGene_->SetPos(VAdd(mTransform.pos, VScale(mTransform.GetForward(), 10.0f)));
		Shot();
		break;
	case Player::PLAYER_STATE::EXP:
		if (spriteAnimator_->IsEnd())
		{
			ChengeState(PLAYER_STATE::END);
		}
		break;
	case Player::PLAYER_STATE::END:
		break;
	default:
		break;
	}
	Quaternion rot = mTransform.rot;
	Quaternion axis;
	axis = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	rot = rot.Mult(axis);
	axis = Quaternion::AngleAxis(AsoUtility::Deg2RadF(90.0f), AsoUtility::AXIS_X);
	rot = rot.Mult(axis);
	parGene_->SetRot(rot);
	parGene_->Update();
	spriteAnimator_->Update();
	for (auto shot : shots_)
	{
		if (!shot->IsAlive())
		{
			continue;
		}
		shot->Update();
	}
}

void Player::Draw(void)
{	
	switch (state_)
	{
	case Player::PLAYER_STATE::NON:
		break;
	case Player::PLAYER_STATE::RUN:
		MV1DrawModel(mTransform.modelId);
		parGene_->Draw();
		break;
	case Player::PLAYER_STATE::EXP:
		break;
	case Player::PLAYER_STATE::END:
		break;
	default:
		break;
	}
	DrawFormatString(0, 32, 0xffffff, "%f,%f,%f", mTransform.pos.x, mTransform.pos.y, mTransform.pos.z);
	spriteAnimator_->Draw();
	for (auto shot : shots_)
	{
		if (!shot->IsAlive())
		{
			continue;
		}
		shot->Draw();
	}
#ifdef _DEBUG
	//DrawSphere3D(mTransform.pos, COLLISION_RADIUS, 16, 0xffffff, 0xffffff, false);
#endif // _DEBUG

}

void Player::Release(void)
{
	parGene_->Release();
	delete parGene_;
	spriteAnimator_->Release();
	delete spriteAnimator_;
	speechBalloon_->Release();
	delete speechBalloon_;
	for (auto shot:shots_)
	{
		shot->Release();
		delete shot;
	}
	shots_.clear();
}

Transform* Player::GetTransform(void)
{
	return &mTransform;
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

void Player::Dead(void)
{
	ChengeState(PLAYER_STATE::EXP);
}

bool Player::isAlive(void)
{
	return isAlive_;
}

bool Player::isEnd(void)
{
	return state_ == PLAYER_STATE::END;
}

void Player::Shot(void)
{
	if (shotInterval_ != 0.0f)
	{
		return;
	}
	if (!CheckHitKey(KEY_INPUT_N))
	{
		return;
	}
	shotInterval_ = SHOT_INTERVAL;
	PlayerShot* shot = new PlayerShot(mSceneManager, &mTransform);
	shot->Create(mTransform.pos, VNorm(mTransform.GetForward()));
	shots_.emplace_back(std::move(shot));
}

const std::vector<PlayerShot*>& Player::GetShots(void) const
{
	return shots_;
}

void Player::ChengeState(PLAYER_STATE state)
{
	if (state_ == state)
	{
		return;
	}
	state_ = state;
	switch (state_)
	{
	case Player::PLAYER_STATE::NON:
		break;
	case Player::PLAYER_STATE::RUN:
		break;
	case Player::PLAYER_STATE::EXP:
		mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW);
		isAlive_ = false;
		spriteAnimator_->Create(mTransform.pos);
		break;
	case Player::PLAYER_STATE::END:
		break;
	default:
		break;
	}
	

}
