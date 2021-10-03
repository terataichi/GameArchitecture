#include <DxLib.h>
#include "Camera.h"
#include "GameCommon.h"
#include "MiniCamera.h"

MiniCamera::MiniCamera(Camera* camera)
{
	mCamera = camera;
	mSize = { 200, 200 };
	mScreen = MakeScreen(mSize.x, mSize.y);
}

MiniCamera::~MiniCamera()
{
}

void MiniCamera::Init(void)
{
	mModel = MV1LoadModel("Model/Camera.mv1");
}

void MiniCamera::Update(void)
{

	VECTOR pos = mCamera->GetPos();
	pos.y += 100.0f;

	MV1SetPosition(mModel, mCamera->GetPos());
	MV1SetScale(mModel, { 3.0f, 3.0f ,3.0f });

	VECTOR angles = mCamera->GetAngle();
	MV1SetRotationXYZ(mModel, angles);

}

void MiniCamera::DrawScreen(void)
{

	SetDrawScreen(mScreen);

	// 画面を初期化
	ClearDrawScreen();

	VECTOR pos = mCamera->GetPos();
	pos.z -= 500.0f;

	// カメラセット
	SetCameraPositionAndAngle(
		pos,
		0.0f,
		0.0f,
		0.0f
	);

	// ライトの設定
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });

	MV1DrawModel(mModel);

	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

}

void MiniCamera::Draw(void)
{

	int width = 10;
	DrawBox(
		SCREEN_SIZE_X - mSize.x - (width * 2),
		0,
		SCREEN_SIZE_X,
		mSize.y + (width * 2),
		0x000000,
		true
	);
	DrawGraph(SCREEN_SIZE_X - mSize.x - width, width, mScreen, false);

}

void MiniCamera::Release(void)
{
	MV1DeleteModel(mModel);
}

int MiniCamera::GetScreen(void)
{
	return mScreen;
}
