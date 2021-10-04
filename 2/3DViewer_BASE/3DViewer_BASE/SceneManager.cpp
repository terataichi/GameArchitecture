#include <chrono>
#include <DxLib.h>
#include "Fader.h"
#include "TitleScene.h"
#include "Camera.h"
#include "MiniCamera.h"
#include "SceneManager.h"
#include "AsoUtility.h"

void SceneManager::Init()
{

	mSceneID = SCENE_ID::TITLE;
	mWaitSceneID = SCENE_ID::NONE;

	mFader = new Fader();
	mFader->Init();

	mCamera = new Camera();

	mMiniCamera = new MiniCamera(mCamera);
	mMiniCamera->Init();

	mScene = new TitleScene(this);
	mScene->Init();

	mIsSceneChanging = false;

	// デルタタイム
	mTickCount = std::chrono::system_clock::now();

	Init3D();
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

	// カメラの更新処理
	mCamera->Update();

	// ミニカメラ
	mMiniCamera->Update();
	mMiniCamera->DrawScreen();

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラの設定
	mCamera->SetBeforeDraw();

	// 描画
	mScene->Draw();
	mCamera->Draw();

	// ミニカメラ
	mMiniCamera->Draw();

	mFader->Draw();

}

void SceneManager::Release(void)
{

	mScene->Release();
	mCamera->Relese();
	delete mScene;

}

void SceneManager::Init3D()
{
	// Zバッファーを有効にする
	SetUseZBuffer3D(true);

	// Zバッファーの書き込みを有効
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	// 無駄なところを描画しない？的な奴
	SetUseBackCulling(true);

	// カメラのクリップ距離の設定
	SetCameraNearFar(0.0f, 1500.0f);

	// ライトの設定
	ChangeLightTypePoint({},1000.0f,10.0f,10.0f, 10.0f);

	// 3Dの背景色
	SetBackgroundColor(0, 139, 50);
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
	return mDeltaTime;
}

Camera* SceneManager::GetCamera()
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
		mScene = new TitleScene(this);
		break;
	default:
		break;
	}

	mScene->Init();

	mWaitSceneID = SCENE_ID::NONE;

}



