#include "DxLib.h"
#include "KeyCheck.h"
#include "SceneManager.h"
#include "Camera.h"
#include "DemoScene.h"
#include "Coin.h"

DemoScene::DemoScene(SceneManager* manager) : SceneBase(manager)
{
}

void DemoScene::Init(void)
{
	mCoin = new Coin(mSceneManager);
}

void DemoScene::Update(void)
{

	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SCENE_ID::TITLE, true);
	}
	mCoin->Update();
}

void DemoScene::Draw(void)
{
	mCoin->Draw();
}

void DemoScene::Release(void)
{
	mCoin->Release();
}
