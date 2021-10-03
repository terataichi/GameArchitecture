#include "Stage.h"
#include "SceneManager.h"
#include <DxLib.h>

Stage::Stage(SceneManager* mng):mSceneMng_(mng)
{
	Init();
}

void Stage::Init()
{
	modelHandle_ = MV1LoadModel("Model/Stage.mv1");
	modelPos_ = { 0.0f,-100.0f,0.0f };

	MV1SetPosition(modelHandle_, modelPos_);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	// XYZのグリッド線描画
	float offset = 500.0f;
	for (int i = 0; i <= 10; i++)
	{
		VECTOR vec1{ -offset,0.0f,-offset + (i * 100.0f) };
		VECTOR vec2{ offset,0.0f, -offset + (i * 100.0f) };
		DrawLine3D(vec1, vec2, 0x00ff00);

		VECTOR vec3{ -offset + (i * 100.0f),0.0f,-offset};
		VECTOR vec4{ -offset + (i * 100.0f),0.0f, offset};
		DrawLine3D(vec3, vec4, 0x0000ff);
	}
	VECTOR vec1{ 0.0f,0.0f,0.0f };
	VECTOR vec2{ 0.0f,offset,0.0f };
	DrawLine3D(vec1, vec2, 0xff0000);

	// モデル描画
	MV1DrawModel(modelHandle_);
}

void Stage::Relese()
{
	MV1DeleteModel(modelHandle_);
}
