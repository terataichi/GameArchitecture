#include "Rock.h"

Rock::Rock()
{
}

void Rock::Init(int modelID, VECTOR pos, VECTOR angle, VECTOR scale)
{
	mTransform.modelId = modelID;
	mTransform.pos = pos;
	mTransform.rot = angle;
	mTransform.scl = scale;
	mTransform.quaRot = Quaternion::Euler(angle);
	mTransform.quaRotLocal = Quaternion();
	mTransform.Update();
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
}
