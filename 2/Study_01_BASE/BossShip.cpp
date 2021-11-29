#include "BossShip.h"
#include "SceneManager.h"
#include "EventShot.h"
#include "Camera.h"
#include "Turret.h"
#include "Player.h"
#include "AsoUtility.h"
#include "ResourceManager.h"
#include "Resource.h"
namespace
{
	constexpr float SPEED_MOVE = 4.0f;
	constexpr float EVENT_TIME = 2.0f;
}

BossShip::BossShip(SceneManager* manager, Player* player) :
	sceneManager_(manager), player_(player)
{
}

void BossShip::Init(void)
{
	deathImage_ = LoadGraph("Image/EndLogo.png");

	VECTOR pos = { 0.0f,0.0f,4000.0f };
	transform_.SetModel(MV1LoadModel("Model/BossShip/BossShip.mv1"));
	transform_.scl = { 2.0f,2.0f,2.0f };
	transform_.pos = pos;
	transform_.quaRot = Quaternion::Euler(AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(210.0f), AsoUtility::Deg2RadF(20.0f));
	transform_.quaRotLocal = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	transform_.Update();

	// ‘O•û
	MakeTurret(
		{ 4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(-18.0f) });
	MakeTurret(
		{ -4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(18.0f) });

	// ‰¡B
	MakeTurret(
		{ 4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(90.0f), 0.0f });
	MakeTurret(
		{ -4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f });

	// Œã•û
	MakeTurret(
		{ 3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(18.0f) });
	MakeTurret(
		{ -3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(-18.0f) });

	MV1SetupCollInfo(transform_.modelId);

	SceneManager::SCENE_ID id = sceneManager_->GetmSceneID();
	switch (id)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChengeState(BOSS_STATE::EVENT);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		ChengeState(BOSS_STATE::BATTLE);
		break;
	default:
		break;
	}

}

void BossShip::Update(void)
{
	switch (state_)
	{
	case BOSS_STATE::EXP:

		endTime_ += sceneManager_->GetDeltaTime();
		if (endTime_ >= 5.0f)
		{
			sceneManager_->ChangeScene(SceneManager::SCENE_ID::TITLE, true);
		}

		break;
	case BOSS_STATE::EVENT:
		auto nor = VNorm(transform_.GetForward());
		auto moveVec = VScale(nor, SPEED_MOVE);
		transform_.pos = VAdd(transform_.pos, moveVec);

		eventShot_->Update();
		if (!eventShot_->IsAlive())
		{
			stepEvent_ -= sceneManager_->GetDeltaTime();
			if (stepEvent_ < 0.0f)
			{
				sceneManager_->ChangeScene(SceneManager::SCENE_ID::BATTLE,true);
				return;
			}
		}
		UpdateTurret();
		break;
	case BOSS_STATE::BATTLE:
		VECTOR dir = transform_.GetForward();
		transform_.pos = VAdd(transform_.pos, VScale(dir, SPEED_MOVE));
		float speed = (DX_PI_F / 180.0f) * sceneManager_->GetDeltaTime();
		Quaternion que = Quaternion::AngleAxis(speed, AsoUtility::AXIS_Y);
		transform_.quaRot = Quaternion::Mult(transform_.quaRot, que);
		UpdateTurret();

		bool active = false;
		for (auto& t : turretList_)
		{
			active |= t->IsAlive();
		}

		if (!active)ChengeState(BOSS_STATE::EXP);

		break;
	}
	transform_.Update();

	MV1RefreshCollInfo(transform_.modelId);
}

void BossShip::UpdateTurret(void)
{
	for (auto turret : turretList_)
	{
		turret->Update();
	}
}

void BossShip::Draw(void)
{
	int num = MV1GetMaterialNum(transform_.modelId);
	COLOR_F cFrom{ 1.0f,1.0f,1.0f,1.0f };
	COLOR_F cTo{ 1.0f,0.1f,0.1f,0.0f };
	int x, y = 0;
	GetDrawScreenSize(&x, &y);

	switch (state_)
	{
	case BOSS_STATE::EVENT:
		MV1DrawModel(transform_.modelId);
		eventShot_->Draw();
		DrawTurret();
		break;
	case BOSS_STATE::BATTLE:
		MV1DrawModel(transform_.modelId);
		DrawTurret();
		break;
	case BOSS_STATE::EXP:                
		t += sceneManager_->GetDeltaTime();
		auto c = AsoUtility::Lerp(cFrom, cTo, t);

		for (int i = 0; i < num; i++)
		{
			MV1SetMaterialDifColor(transform_.modelId, i, c);
		}
		MV1DrawModel(transform_.modelId);
		DrawGraph(x / 2 - x / 4, y / 3, deathImage_, true);
		break;
	default:
		break;
	}
}

void BossShip::DrawTurret(void)
{
	for (auto turret : turretList_)
	{
		turret->Draw();
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
	for (auto turret : turretList_)
	{
		turret->Release();
		delete turret;
	}
	turretList_.clear();
}

std::vector<Turret*> BossShip::GetTurret(void)
{
	return turretList_;
}

const int& BossShip::GetModelID(void)
{
	return transform_.modelId;
}

void BossShip::MakeTurret(VECTOR localPos, VECTOR localAngle)
{
	Turret* turret = new Turret(sceneManager_, player_->GetTransform(), &transform_, localPos, localAngle);
	turret->Init();

	turretList_.emplace_back(turret);
}

void BossShip::ChengeState(BOSS_STATE state)
{
	if (state_ == state)
	{
		return;
	}
	state_ = state;
	switch (state_)
	{
	case BossShip::BOSS_STATE::NON:
		break;
	case BossShip::BOSS_STATE::EVENT:
		stepEvent_ = EVENT_TIME;
		eventShot_ = new EventShot(sceneManager_, &transform_);
		auto norVec = VNorm(VSub(sceneManager_->GetCamera()->GetPos(), transform_.pos));
		eventShot_->Create(transform_.pos, norVec);
		break;
	case BossShip::BOSS_STATE::BATTLE:
		break;
	case BossShip::BOSS_STATE::EXP:
		break;
	case BossShip::BOSS_STATE::END:
		break;
	default:
		break;
	}
}
