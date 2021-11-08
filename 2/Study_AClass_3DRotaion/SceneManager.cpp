#include <chrono>
#include "DxLib.h"
#include "Fader.h"
#include "DemoScene.h"
#include "Camera.h"
#include "SceneManager.h"

void SceneManager::Init()
{

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera(this);
	mCamera->Init();

	mScene = new DemoScene(this);
	mScene->Init();

	mIsSceneChanging = false;

	// デルタタイム
	mTickCount = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// クリップ距離を設定する
	SetCameraNearFar(0.0f, 1500.0f);

	// ライトの設定
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(false);

	// 背景色設定
	SetBackgroundColor(0, 139, 139);

	// カメラ設定
	// 初期状態では視点の位置が、 
	// x = 320.0f, y = 240.0f, z = (画面のサイズによって変化)
	// 注視点の位置は、
	// x = 320.0f, y = 240.0f, z = 1.0f、
	// カメラの上方向は
	// x = 0.0f, y = 1.0f, z = 0.0f
	// つまり画面のＸＹ平面上の中心に居てＺ軸のプラス方向を見るようなカメラになっています。
	//SetCameraPositionAndAngle(
	//	{ 0.0f, 200, -500.0f },
	//	30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f
	//);

}

void SceneManager::Update(void)
{

	if (mScene == nullptr)
	{
		return;
	}

	// デルタタイム
	auto tickCount = std::chrono::system_clock::now();
	mDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(tickCount - mTickCount).count() / 1000000000.0f;
	mTickCount = tickCount;

	mFader->Update();
	if (mIsSceneChanging)
	{
		Fader::FADE_STATE fState = mFader->GetState();
		switch (fState)
		{
		case Fader::FADE_STATE::FADE_IN:
			if (mFader->IsEnd())
			{
				mFader->SetFade(Fader::FADE_STATE::NONE);
				mIsSceneChanging = false;
			}
			break;
		case Fader::FADE_STATE::FADE_OUT:
			if (mFader->IsEnd())
			{
				DoChangeScene();
				mFader->SetFade(Fader::FADE_STATE::FADE_IN);
			}
			break;
		default:
			break;
		}
	}
	else 
	{
		mScene->Update();
	}

	// カメラ更新ステップ
	mCamera->Update();

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	mCamera->SetBeforeDraw();

	// 描画
	mScene->Draw();
	mCamera->Draw();

	mFader->Draw();
}

void SceneManager::Release(void)
{

	mScene->Release();
	delete mScene;

	mCamera->Release();
	delete mCamera;

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{
	mWaitSceneID = nextId;

	if (isFading)
	{
		mFader->SetFade(Fader::FADE_STATE::FADE_OUT);
		mIsSceneChanging = true;
	}
	else 
	{
		DoChangeScene();
	}

}

float SceneManager::GetDeltaTime(void)
{
	//return mDeltaTime;
	return 0.016f;
}

Camera* SceneManager::GetCamera(void)
{
	return mCamera;
}

void SceneManager::DoChangeScene(void)
{
	mScene->Release();

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new DemoScene(this);
		break;
	default:
		break;
	}

	mScene->Init();

	mWaitSceneID = SCENE_ID::NONE;

}



