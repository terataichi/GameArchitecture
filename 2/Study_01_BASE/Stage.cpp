#include "Stage.h"
#include "AsoUtility.h"
#include "SceneManager.h"
namespace
{
	constexpr float grid_length = 3000.0f;
	constexpr float grid_term = 100.0;
}

Stage::Stage(SceneManager* manager):mSceneManager(manager)
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
	mModelDungeon = MV1LoadModel("Model/Rock/Dungeon.mv1");
	MV1SetScale(mModelDungeon, { 1.0f,1.0f,1.0f });
	MV1SetPosition(mModelDungeon, { 1000.0f,400.0f,1300.0f });
	MV1SetRotationXYZ(mModelDungeon, { 0.0f,180.0f,0.0f });

	mBossModel = MV1LoadModel("Model/BossShip/BossShip.mv1");
	MV1SetScale(mBossModel, { 2.0f,2.0f,2.0f });
	MV1SetPosition(mBossModel, { 14500.0f,-800.0f,6500.0f });
	MV1SetRotationXYZ(mBossModel, { 0.0f,180.0f,0.0f });
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

	MV1DrawModel(mModelDungeon);
	MV1DrawModel(mBossModel);

	DrawSphere3D(MV1GetPosition(mBossModel), RADIUS, 10, 0xffffff, 0xffffff, false);
}

void Stage::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
	MV1DeleteModel(mModelDungeon);
	MV1DeleteModel(mBossModel);

}

int Stage::GetModelDungeon()
{
	return mModelDungeon;
}

int Stage::GetModelBoss()
{
	return mBossModel;
}

