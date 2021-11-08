#include <chrono>
#include "DxLib.h"
#include "Fader.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "ParticleGenerator.h"

void SceneManager::Init()
{

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera(this);
	mCamera->Init();

	mResourceManager = new ResourceManager();
	mResourceManager->Init();

	particleGenerator_ = new ParticleGenerator(this, { 0,0,0 }, 20);
	particleGenerator_->Init();

	mScene = new TitleScene(this);
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

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	//SetCameraNearFar(0.0f, 50.0f);

	// ライトの設定
	//ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(false);

	// 背景色設定
	SetBackgroundColor(0, 0, 0);
	//SetBackgroundColor(0, 100, 100);

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

	particleGenerator_->Update();

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
	particleGenerator_->Draw();

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		break;
	case SCENE_ID::GAME:
		// デバッグ描画
		mCamera->DrawDebug();
		// UI描画
		mCamera->DrawUI();
		break;
	}

	mFader->Draw();

}

void SceneManager::Release(void)
{

	delete mFader;

	mScene->Release();
	delete mScene;

	mCamera->Release();
	delete mCamera;

	mResourceManager->Release();
	delete mResourceManager;

	particleGenerator_->Release();
	delete particleGenerator_;
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

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return mSceneID;
}

float SceneManager::GetDeltaTime(void)
{
	//return 0.016f;
	return mDeltaTime;
}

Camera* SceneManager::GetCamera(void)
{
	return mCamera;
}

ResourceManager* SceneManager::GetResource(void)
{
	return mResourceManager;
}

void SceneManager::ResetDeltaTime(void)
{
	mDeltaTime = 0.016f;
	mTickCount = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	mResourceManager->Release();
	mScene->Release();

	mSceneID = mWaitSceneID;

	switch (mSceneID)
	{
	case SCENE_ID::TITLE:
		mScene = new TitleScene(this);
		break;
	case SCENE_ID::GAME:
		mScene = new GameScene(this);
		break;
	case SCENE_ID::EVENT:
		break;
	case SCENE_ID::BATTLE:
		break;
	default:
		break;
	}

	mScene->Init();
	
	ResetDeltaTime();

	mWaitSceneID = SCENE_ID::NONE;

}



