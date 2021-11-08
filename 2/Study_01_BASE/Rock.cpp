#include "Rock.h"

Rock::Rock(int modelID, VECTOR pos, VECTOR angle, VECTOR Scale)
{
	mTransform.modelId = modelID;
	mTransform.pos = pos;
	mTransform.quaRot = Quaternion::Euler(angle);
	mTransform.scl = Scale;
	mTransform.quaRotLocal = Quaternion();
	mTransform.Update();
	Init();
}

Rock::~Rock()
{
}

void Rock::Init(void)
{
}

void Rock::Update(void)
{
}

void Rock::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void Rock::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
}
