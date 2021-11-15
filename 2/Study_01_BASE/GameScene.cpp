#include <cmath>
#include "DxLib.h"
#include "GameCommon.h"
#include "KeyCheck.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameScene.h"
#include "SpaceDome.h"
#include "Stage.h"
#include "Player.h"
#include "RockManager.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "SpriteAnimator.h"
#include "SpeechBalloon.h"
GameScene::GameScene(SceneManager* manager) : SceneBase(manager)
{
}

void GameScene::Init(void)
{
	mSpaceDome = new SpaceDome(mSceneManager);
	mSpaceDome->Init();

	mStage = new Stage(mSceneManager);
	mStage->Init();

	mPlayer = new Player(mSceneManager);
	mPlayer->Init();

	rockManager = new RockManager(mSceneManager, mPlayer);
	rockManager->Init();

	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW_SPRING);
	mSceneManager->GetCamera()->SetPlayer(mPlayer);

	mSpaceDome->SetPlayer(mPlayer);

	resetTime_ = 0.0f;
}

void GameScene::Update(void)
{
	mSpaceDome->Update();
	mPlayer->Update();
	rockManager->Update();

	VECTOR pos = MV1GetPosition(mStage->GetModelBoss());
	
	if (VSquareSize(VSub(pos, mPlayer->GetTransForm().pos)) < mStage->RADIUS * mStage->RADIUS)
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}
	if (mPlayer->GetState() == Player::STATE::Destroy)
	{
		resetTime_ += mSceneManager->GetDeltaTime();

		if (resetTime_ > RESTART_TIME)
		{
			mSceneManager->ChangeScene(SceneManager::SCENE_ID::GAME, true);
		}
	}
	else
	{
		auto info = MV1CollCheck_Sphere(mStage->GetModelDungeon(), -1,
			mPlayer->GetTransForm().pos, Player::COLLISION_RADIUS);

		if (info.HitNum > 0)
		{
			if (mPlayer->GetState() != Player::STATE::Destroy)
			{
				//mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FOLLOW_SPRING);
				mPlayer->SetState(Player::STATE::Destroy);
			}
		}
	}


}

void GameScene::Draw(void)
{

	mSpaceDome->Draw();
	mStage->Draw();
	mPlayer->Draw();
	rockManager->Draw();
	mPlayer->GetSpeech()->Draw();
}

void GameScene::Release(void)
{
	mSpaceDome->Release();
	mStage->Release();
	mPlayer->Release();
	rockManager->Release();
}
