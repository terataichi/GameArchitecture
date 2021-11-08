#include "Stage.h"
#include "SceneManager.h"
#include <DxLib.h>
Stage::Stage(SceneManager* scene)
{
	mSceneManager = scene;
}

Stage::~Stage()
{
}

void Stage::Init(void)
{
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	DrawGrid();
}

void Stage::Release(void)
{
}

void Stage::DrawGrid()
{
	// XYZ‚ÌƒOƒŠƒbƒhü•`‰æ
	int max = 100;
	float offset = 50.0f * max;
	for (int i = 0; i <= max; i++)
	{
		VECTOR vec1{ -offset,0.0f,-offset + (i * 100.0f) };
		VECTOR vec2{ offset,0.0f, -offset + (i * 100.0f) };
		DrawLine3D(vec1, vec2, 0xff0000);

		VECTOR vec3{ -offset + (i * 100.0f),0.0f,-offset };
		VECTOR vec4{ -offset + (i * 100.0f),0.0f, offset };
		DrawLine3D(vec3, vec4, 0x0000ff);
	}
	VECTOR vec1{ 0.0f,0.0f,0.0f };
	VECTOR vec2{ 0.0f,offset,0.0f };
	DrawLine3D(vec1, vec2, 0x00ff00);
}
