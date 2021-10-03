#include <DxLib.h>
#include "KeyCheck.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Camera.h"
#include "Stage.h"
#include "AsoUtility.h"
#include "Unit.h"

TitleScene::TitleScene(SceneManager* manager) : SceneBase(manager)
{
}

void TitleScene::Init(void)
{
	mStage_ = new Stage(mSceneManager);
	mUnit_ = new Unit(mSceneManager);
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
}

void TitleScene::Draw(void)
{
	mStage_->Draw();
	mUnit_->Draw();
	DrawDebug();
}

void TitleScene::Release(void)
{
	mStage_->Relese();
	delete mStage_;
	mUnit_->Relese();
	delete mUnit_;
}

void TitleScene::DrawDebug(void)
{

	Camera* camera = mSceneManager->GetCamera();

	auto pos = camera->GetPos();
	auto angle = camera->GetAngle();

	DrawFormatString(0, 10, 0xffffff, "�J�������W	:(%.1f,%.1f,%.1f)",
		pos.x, pos.y, pos.z);
	
	DrawFormatString(0, 30, 0xffffff, "�J�����p�x	:(%.1f,%.1f,%.1f)",
		AsoUtility::Rad2DegD(angle.x), AsoUtility::Rad2DegD(angle.y), AsoUtility::Rad2DegD(angle.z));

	pos = mUnit_->GetPos();
	angle = mUnit_->GetAngle();
	DrawFormatString(0, 50, 0xffffff, "�L�������W	:(%.1f,%.1f,%.1f)",
		pos.x, pos.y, pos.z);

	DrawFormatString(0, 70, 0xffffff, "�L�����p�x	:(%.1f,%.1f,%.1f)",
		AsoUtility::Rad2DegD(angle.x), AsoUtility::Rad2DegD(angle.y), AsoUtility::Rad2DegD(angle.z));
}