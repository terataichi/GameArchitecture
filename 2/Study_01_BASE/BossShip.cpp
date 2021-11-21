#include "BossShip.h"
#include "SceneManager.h"
#include "EventShot.h"
#include "Camera.h"
#include "AsoUtility.h"

BossShip::BossShip(SceneManager* manager, Player* player) :
	sceneManager_(manager), player_(player)
{
	Init();
}

void BossShip::Init(void)
{
	VECTOR pos = { 0.0f,0.0f,4000.0f };
	transform_.SetModel(MV1LoadModel("Model/BossShip/BossShip.mv1"));
	transform_.scl = { 2.0f,2.0f,2.0f };
	transform_.pos = pos;
	transform_.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(210.0f), AsoUtility::Deg2RadF(20.0f));
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	transform_.Update();

	SceneManager::SCENE_ID id = sceneManager_->GetmSceneID();
	switch (id)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChengeState(Boss_State::Event);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		ChengeState(Boss_State::Battle);
		break;
	default:
		break;
	}

}

void BossShip::Update(void)
{
	switch (state_)
	{
	case Boss_State::Event:
		auto nor = VNorm(transform_.GetForward());
		auto moveVec = VScale(nor, SPEED_MOVE);
		transform_.pos = VAdd(transform_.pos, moveVec);

		eventShot_->Update();
		if (!eventShot_->IsAlive())
		{
			stepEvent_ -= sceneManager_->GetDeltaTime();
			if (stepEvent_ < 0.0f)
			{
				sceneManager_->ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
				return;
			}
		}
		break;
	case Boss_State::Battle:
		break;
	case Boss_State::Exp:
		break;
	default:
		break;
	}
	transform_.Update();
}

void BossShip::Draw(void)
{
	switch (state_)
	{
	case Boss_State::Event:
		MV1DrawModel(transform_.modelId);
		eventShot_->Draw();
		break;
	case Boss_State::Battle:
		break;
	case Boss_State::Exp:
		break;
	default:
		break;
	}
}

void BossShip::Release(void)
{
	MV1DeleteModel(transform_.modelId);
	if (eventShot_ != nullptr)
	{
		eventShot_->Release();
		delete eventShot_;
	}
}

void BossShip::ChengeState(Boss_State state)
{
	if (state_ == state)
	{
		return;
	}
	state_ = state;
	switch (state_)
	{
	case BossShip::Boss_State::Non:
		break;
	case BossShip::Boss_State::Event:
		stepEvent_ = EVENT_TIME;
		eventShot_ = new EventShot(sceneManager_, &transform_);
		auto norVec = VNorm(VSub(sceneManager_->GetCamera()->GetPos(), transform_.pos));
		eventShot_->Create(transform_.pos, norVec);
		break;
	case BossShip::Boss_State::Battle:
		break;
	case BossShip::Boss_State::Exp:
		break;
	case BossShip::Boss_State::End:
		break;
	default:
		break;
	}
}
