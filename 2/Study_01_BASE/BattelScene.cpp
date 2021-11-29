#include <DxLib.h>
#include "BattelScene.h"

#include "KeyCheck.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Player.h" 
#include "PlayerShot.h"
#include "RockManager.h"
#include "BossShip.h"
#include "SpaceDome.h"
#include "Turret.h"
#include "TurretShot.h"
#include "AsoUtility.h"
namespace
{
	constexpr float RESTART_TIME = 3.0f;
}

BattelScene::BattelScene(SceneManager* manager) :
	SceneBase(manager)
{
}

void BattelScene::Init(void)
{
	player_ = new Player(mSceneManager);
	player_->Init();
	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();
	mSpaceDome->SetPlayer(player_);
	rockManager_ = new RockManager(mSceneManager, player_);
	rockManager_->Init();
	rockManager_->Update();
	boss_ = new BossShip(mSceneManager, player_);
	boss_->Init();
	mSceneManager->GetCamera()->SetPlayer(player_);
	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW_SPRING);

	playerDeadTime_ = RESTART_TIME;
}

void BattelScene::Update(void)
{
	auto Collision = [](const int& handle, const VECTOR& center, const float& rad)
	{
		auto info = MV1CollCheck_Sphere(handle, -1, center, rad);
		bool isHit = info.HitNum != 0 ? true : false;
		MV1CollResultPolyDimTerminate(info);
		return isHit;
	};
	// ƒV[ƒ“‘JˆÚ
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}
	player_->Update();
	boss_->Update();
	mSpaceDome->Update();
	rockManager_->Update();

	if (player_->isAlive())
	{
		auto info = MV1CollCheck_Sphere(boss_->GetModelID(), -1, player_->GetTransform()->pos, Player::COLLISION_RADIUS);

		if (info.HitNum)
		{
			player_  ->Dead();
		}
		MV1CollResultPolyDimTerminate(info);

	}
	else
	{
		if (player_->isEnd())
		{
			playerDeadTime_ -= mSceneManager->GetDeltaTime();
			if (playerDeadTime_ <= 0.0f)
			{
				mSceneManager->ChangeScene(SceneManager::SCENE_ID::BATTLE, true);
			}
		}
	}
	auto shots = player_->GetShots();
	auto turret = boss_->GetTurret();

	for (auto shot : shots)
	{
		if (Collision(boss_->GetModelID(), shot->GetPos(), PlayerShot::COLLISION_RADIUS))
		{
			shot->CreateExplosion();
		}

		for (auto& t : turret)
		{
			if (!t->IsAlive())
			{
				continue;
			}

			if (AsoUtility::IsHitSpheres(t->GetPos(),
				t->COLLISION_RADIUS,
				shot->GetPos(),
				shot->GetCollisionRadius()))
			{
				t->Damage();
				shot->CreateExplosion();
				break;
			}
		}
	}

	for (auto& t : turret)
	{
		auto tShot = t->GetShot();
		for (auto s : tShot)
		{
			if (!s->IsAlive())
			{
				continue;
			}

			if (AsoUtility::IsHitSpheres(player_->GetTransform()->pos,
				Player::COLLISION_RADIUS,
				s->GetPos(),
				s->GetCollisionRadius()))
			{

				player_->Dead();
			}

		}
	}
}

void BattelScene::Draw(void)
{
	mSpaceDome->Draw();
	boss_->Draw();
	player_->Draw();
	rockManager_->Draw();

}

void BattelScene::Release(void)
{
	player_->Release();
	delete player_;
	boss_->Release();
	delete boss_;
	mSpaceDome->Release();
	delete mSpaceDome;
	rockManager_->Release();
	delete rockManager_;
}
