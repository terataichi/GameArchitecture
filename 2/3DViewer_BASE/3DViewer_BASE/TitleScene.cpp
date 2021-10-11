#include <DxLib.h>
#include "KeyCheck.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Camera.h"
#include "Stage.h"
#include "AsoUtility.h"
#include "Unit.h"
#include "RollBall.h"
#include "Enemy.h"

TitleScene::TitleScene(SceneManager* manager) : SceneBase(manager)
{
}

void TitleScene::Init(void)
{
	mStage_ = new Stage(mSceneManager);
	mUnit_ = new Unit(mSceneManager);
	mRollBall_ = new RollBall(mSceneManager, mUnit_);
	mEnemy_ = new Enemy(mSceneManager,mUnit_);
	mSceneManager->GetCamera()->SetUnit(mUnit_);
	//mStage_->Init();
}

void TitleScene::Update(void)
{

	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SCENE_ID::TITLE, true);
	}
	mStage_->Update();
	mUnit_->Update();
	mRollBall_->Update();
	mEnemy_->Update();
}

void TitleScene::Draw(void)
{
	mStage_->Draw();
	mUnit_->Draw();
	mRollBall_->Draw();
	mEnemy_->Draw();
	DrawDebug();
}

void TitleScene::Release(void)
{
	mStage_->Relese();
	delete mStage_;
	mUnit_->Relese();
	delete mUnit_;
	mRollBall_->Relese();
	delete mRollBall_;
}

void TitleScene::DrawDebug(void)
{

	Camera* camera = mSceneManager->GetCamera();

	auto pos = camera->GetPos();
	auto angle = camera->GetAngle();

	DrawFormatString(0, 10, 0xffffff, "カメラ座標	:(%.1f,%.1f,%.1f)",
		pos.x, pos.y, pos.z);
	
	DrawFormatString(0, 30, 0xffffff, "カメラ角度	:(%.1f,%.1f,%.1f)",
		AsoUtility::Rad2DegD(angle.x), AsoUtility::Rad2DegD(angle.y), AsoUtility::Rad2DegD(angle.z));

	pos = mUnit_->GetPos();
	angle = mUnit_->GetAngle();
	DrawFormatString(0, 50, 0xffffff, "キャラ座標	:(%.1f,%.1f,%.1f)",
		pos.x, pos.y, pos.z);

	DrawFormatString(0, 70, 0xffffff, "キャラ角度	:(%.1f,%.1f,%.1f)",
		AsoUtility::Rad2DegD(angle.x), AsoUtility::Rad2DegD(angle.y), AsoUtility::Rad2DegD(angle.z));
	
	DrawFormatString(0, 100, 0xffffff, "玉角度	:(%.1f,)",
		AsoUtility::Rad2DegD(mRollBall_->GetAngle()));
}
