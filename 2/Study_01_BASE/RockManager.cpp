#include "RockManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Rock.h"
#include "AsoUtility.h"
RockManager::RockManager(SceneManager* mng, Transform* trams)
{
	mSceneManager = mng;
	mPlayerTransform = trams;
	mResourceManager = mSceneManager->GetResource();
}

RockManager::~RockManager()
{
}

void RockManager::Init(void)
{
}

void RockManager::Update(void)
{
	// プレイヤーの位置を見て、周囲27ますのマップを生成を
	// 行うか判断する

	// プレイヤーの座標をマップ座標に変換
	VECTOR mapPos = mPlayerTransform->pos;

	IntVector3 intVec3 = { 0,0,0 };

	if (mMapRocks.count(intVec3) == 0)
	{
		std::vector<Rock*> rocks;

		rocks.push_back(CreateRandom(intVec3));

		// マップ管理に加える
		mMapRocks.emplace(intVec3, rocks);
	}


}

void RockManager::Draw(void)
{
	for (auto& pair : mMapRocks)
	{
		for (auto& rock : pair.second)
		{
			if (rock == nullptr)
			{
				continue;
			}
			rock->Draw();
		}
	}
}

void RockManager::Release(void)
{
	for (auto& pair : mMapRocks)
	{
		for (auto& rock : pair.second)
		{
			if (rock == nullptr)
			{
				continue;
			}
			rock->Release();
		}
		pair.second.clear();
	}
	mMapRocks.clear();
}

Rock* RockManager::CreateRandom(IntVector3 mapPos)
{
	// 岩のモデル
	int r = GetRand(1);
	int id = -1;
	
	if (r == 0)
	{
		id = mResourceManager->LoadModelDuplicate(ResourceManager::SRC::ROCK01);
	}
	else
	{
		id = mResourceManager->LoadModelDuplicate(ResourceManager::SRC::ROCK02);
	}

	// 位置
	VECTOR pos;
	

	// ランダム角度
	VECTOR angle{
	AsoUtility::Deg2RadF(GetRand(360)),
	AsoUtility::Deg2RadF(GetRand(360)),
	0.0f
	};

	// ランダム大きさ
	VECTOR scale
	{
	2 + GetRand(8),
	2 + GetRand(8),
	2 + GetRand(8),
	};

	return new Rock(id, {}, angle, {});
}
