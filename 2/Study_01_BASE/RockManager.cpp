#include "RockManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Rock.h"
#include "RandomEngine.h"
#include "Player.h"

namespace
{
	constexpr float MAP_DIV_SIZE = 3000.0f;
}

RockManager::RockManager(SceneManager* sceneManager, Player* player)
{
	mSceneManager = sceneManager;
	mPlayer = player;
	mResourceManager = sceneManager->GetResource();
}

void RockManager::Init(void)
{
	mResourceManager->Load(ResourceManager::SRC::ROCK01);
	mResourceManager->Load(ResourceManager::SRC::ROCK02);
	Update();
}

void RockManager::Update(void)
{
	RandomRockInstance();
}

void RockManager::Draw(void)
{
	for (const auto& rocks : mMapRocks)
	{
		for (const auto rock : rocks.second)
		{
			rock->Draw();
		}
	}
}

void RockManager::Release(void)
{
	for (auto& rocks : mMapRocks)
	{
		for (auto& rock : rocks.second)
		{
			rock->Release();
		}
	}
	mMapRocks.clear();
}

void RockManager::RandomRockInstance(void)
{
	VECTOR mapPos = mPlayer->GetTransform()->pos;
	IntVector3 cMapPos = { static_cast<int>(mapPos.x / MAP_DIV_SIZE) ,static_cast<int>(mapPos.y / MAP_DIV_SIZE) ,static_cast<int>(mapPos.z / MAP_DIV_SIZE )};
	IntVector3 tmpPos = {};

	std::vector<Rock*> rocks;
	for (int i = -1; i < 2; i++)
	{
		tmpPos.x = cMapPos.x + i;
		for (int j = -1; j < 2; j++)
		{
			tmpPos.y = cMapPos.y = j;
			for (int k = -1; k < 2; k++)
			{
				tmpPos.z = cMapPos.z + k;
				if (mMapRocks.count(tmpPos) != 0)
				{
					break;
				}
				for (int n = 0; n < 30; n++)
				{
					rocks.emplace_back(CreateRock(tmpPos));
				}
				mMapRocks.try_emplace(tmpPos, rocks);
				rocks.clear();
			}
		}
	}

}

Rock* RockManager::CreateRock(IntVector3 mPos)
{
	int random = RandomEngine::RandomInt(0, 1);
	int modelID = -1;
	switch (random)
	{
	case 0:
		modelID = mResourceManager->LoadModelDuplicate(ResourceManager::SRC::ROCK01);
		break;
	case 1:
		modelID = mResourceManager->LoadModelDuplicate(ResourceManager::SRC::ROCK02);
		break;
	default:
		break;
	}

	Rock* re = new Rock();

	float scale = RandomEngine::RandomFloat(5.0f, 20.0f);

	VECTOR pos = { RandomEngine::RandomFloat(-MAP_DIV_SIZE / 2.0f, MAP_DIV_SIZE / 2.0f),RandomEngine::RandomFloat(-MAP_DIV_SIZE / 2.0f, MAP_DIV_SIZE / 2.0f),RandomEngine::RandomFloat(-MAP_DIV_SIZE / 2.0f, MAP_DIV_SIZE / 2.0f) };
	VECTOR angle = { RandomEngine::RandomFloat(0.0f, DX_TWO_PI_F),RandomEngine::RandomFloat(0.0f, DX_TWO_PI_F),0.0f };
	VECTOR scales = { scale ,scale ,scale };

	pos = VAdd(pos, VGet(mPos.x * MAP_DIV_SIZE, mPos.y * MAP_DIV_SIZE, mPos.z * MAP_DIV_SIZE));

	re->Init(
		modelID,
		pos,
		angle,
		scales
	);

	return re;
}
