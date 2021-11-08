#include "SpaseDome.h"
#include "AsoUtility.h"
#include <DxLib.h>
SpaseDome::SpaseDome(SceneManager* scene, Transform* ship)
	: mSceneManager(scene), mShipTransform(ship)
{
	Init();
}

SpaseDome::~SpaseDome()
{
}

void SpaseDome::Init(void)
{
	mTransform.SetModel(MV1LoadModel("Model/SkyDome/SkyDome.mv1"));
	float scl = 1.0f;
	mTransform.scl = { scl ,scl ,scl };
	mTransform.pos = AsoUtility::VECTOR_ZERO;
	mTransform.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	mTransform.quaRotLocal = Quaternion();
	
	mState = STATE::FOLLOW;

	mTransform.Update();
}

void SpaseDome::Update(void)
{
	switch (mState)
	{
	case SpaseDome::STATE::STAY:
		break;
	case SpaseDome::STATE::FOLLOW:
		if (mShipTransform != nullptr)
		{
			mTransform.pos = mShipTransform->pos;
		}
		break;
	default:
		break;
	}

	mTransform.Update();
}

void SpaseDome::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void SpaseDome::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
}

void SpaseDome::ChangeState(STATE state)
{
	mState = state;
}
