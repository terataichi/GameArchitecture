#include "BossShip.h"
#include "Quaternion.h"
#include "AsoUtility.h"
#include "SceneManager.h"
#include "EventShot.h"
BossShip::BossShip(SceneManager* manager, Player* playerShip):
	mSceneMng(manager),mPlayer(playerShip)
{
}

void BossShip::Init(void)
{
	VECTOR pos = { 0.0f,0.0f,4000.0f };

	mTransform.SetModel(MV1LoadModel("Model/BossShip/BossShip.mv1"));
	mTransform.scl = { 2.0f,2.0f,2.0f };
	mTransform.pos = pos;
	mTransform.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(20.0f),
		AsoUtility::Deg2RadF(210.0f),
		AsoUtility::Deg2RadF(20.0f)
		);
	mTransform.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(.0f)
	);

	auto id = mSceneMng->GetmSceneID();

	switch (id)
	{
	case SceneManager::SCENE_ID::NONE:
		state_ = STATE::None;
		break;
	case SceneManager::SCENE_ID::EVENT:
		state_ = STATE::Event;
		shot = new EventShot(mSceneMng, &mTransform);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		state_ = STATE::Battle;
		break;
	default:

	}
}

void BossShip::Update(void)
{

	switch (state_)
	{
	case BossShip::STATE::None:
		break;
	case BossShip::STATE::Event:
		VECTOR dir = mTransform.GetForward();
		//mTransform.pos = VAdd(mTransform.pos,dir)

		break;
	case BossShip::STATE::Battle:
		break;
	case BossShip::STATE::Destroy:
		break;
	case BossShip::STATE::End:
		break;
	default:
		break;
	}

	mTransform.Update();
}

void BossShip::Draw(void)
{
	MV1DrawModel(mTransform.modelId);
}

void BossShip::Release(void)
{
}
