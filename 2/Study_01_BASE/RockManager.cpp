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
	// �v���C���[�̈ʒu�����āA����27�܂��̃}�b�v�𐶐���
	// �s�������f����

	// �v���C���[�̍��W���}�b�v���W�ɕϊ�
	VECTOR mapPos = mPlayerTransform->pos;

	IntVector3 intVec3 = { 0,0,0 };

	if (mMapRocks.count(intVec3) == 0)
	{
		std::vector<Rock*> rocks;

		rocks.push_back(CreateRandom(intVec3));

		// �}�b�v�Ǘ��ɉ�����
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
	// ��̃��f��
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

	// �ʒu
	VECTOR pos;
	

	// �����_���p�x
	VECTOR angle{
	AsoUtility::Deg2RadF(GetRand(360)),
	AsoUtility::Deg2RadF(GetRand(360)),
	0.0f
	};

	// �����_���傫��
	VECTOR scale
	{
	2 + GetRand(8),
	2 + GetRand(8),
	2 + GetRand(8),
	};

	return new Rock(id, {}, angle, {});
}
