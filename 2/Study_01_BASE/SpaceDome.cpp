#include "SpaceDome.h"
#include "AsoUtility.h"
#include "Player.h"

SpaceDome::SpaceDome(SceneManager* manager)
{
	
}

void SpaceDome::Init(void)
{
	mTransform.SetModel(MV1LoadModel("Model/SkyDome/SkyDome.mv1"));
	float scale = 1.0;
	mTransform.scl = { scale,scale,scale };
	mTransform.quaRot = Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	mTransform.quaRotLocal = Quaternion();
	mTransform.pos = AsoUtility::VECTOR_ZERO;

	MV1SetWriteZBuffer(mTransform.modelId, false);

	state_ = state::FOLLOW;
	player_ = nullptr;

	mTransform.Update();
}

void SpaceDome::Update(void)
{
	switch (state_)
	{
	case state::WAIT:
		break;
	case state::FOLLOW:
		if (player_ != nullptr)
		{
			mTransform.pos = player_->GetTransForm().pos;
		}
		break;
	default:
		break;
	}
	mTransform.Update();
}

void SpaceDome::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void SpaceDome::Release(void)
{
	MV1DeleteModel(mTransform.modelId);
}

void SpaceDome::SetPlayer(Player* player)
{
	player_ = player;
}
