#include "Stage.h"
#include "AsoUtility.h"

namespace
{
	constexpr float grid_length = 3000.0f;
	constexpr float grid_term = 100.0;
}

Stage::Stage(SceneManager* manager)
{

}

void Stage::Init(void)
{
	/*mTransform.SetModel(MV1LoadModel("Model/SkyDome/SkyDome.mv1"));
	float scale = 1.0;
	mTransform.scl = { scale,scale,scale };
	mTransform.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	mTransform.quaRotLocal = Quaternion();
	mTransform.pos = AsoUtility::VECTOR_ZERO;

	mTransform.Update();*/
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	int num = grid_length / grid_term;
	for (int i = -num; i < num; i++)
	{
		//X
		DrawLine3D(VGet(-grid_length, 0, grid_term * i), VGet(grid_length, 0, grid_term * i), 0xff0000);
		DrawLine3D(VGet(grid_term * i, 0, -grid_length), VGet(grid_term * i, 0, grid_length), 0x0000ff);
	}

	DrawLine3D(VGet(0, -grid_length, 0), VGet(0, grid_length, 0), 0x00ff00);
}

void Stage::Release(void)
{
	//MV1DeleteModel(mTransform.modelId);
}

