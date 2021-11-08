#include <cmath>
#include "DxLib.h"
#include "GameCommon.h"
#include "KeyCheck.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "Camera.h"
#include "GameScene.h"
#include "SpaseDome.h"
#include "Stage.h"
#include "PlayerShip.h"

GameScene::GameScene(SceneManager* manager) : SceneBase(manager)
{

}

void GameScene::Init(void)
{
	mPlayerShip = new PlayerShip(mSceneManager);
	mSpaseDome = new SpaseDome(mSceneManager, mPlayerShip->GetTransform());
	mStage = new Stage(mSceneManager);

	mSceneManager->GetCamera()->SetShip(mPlayerShip->GetTransform());
	mSceneManager->GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
}

void GameScene::Update(void)
{
	// �V�[���J��
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::EVENT, true);
	}
	// �v���C���[���ŏ��ɍX�V
	// �v���C���[�̈ړ��ɍ��킹�ď���������̂���������
	mPlayerShip->Update();
	mSpaseDome->Update();
	mStage->Update();
}

void GameScene::Draw(void)
{
	mSpaseDome->Draw();
	mStage->Draw();
	mPlayerShip->Draw();
}

void GameScene::Release(void)
{
}
