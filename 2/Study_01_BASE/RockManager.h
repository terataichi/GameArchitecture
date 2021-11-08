#pragma once
#include <map>
#include <vector>
#include "Transform.h"
#include "IntVector3.h"

class SceneManager;
class ResourceManager;
class Rock;

class RockManager
{
public:
	// 1マップ当たりのサイズ
	static constexpr int MAP_SIZE = 2000;
	// １マップ当たりに生成する岩野和
	static constexpr int NUM_CREATE = 30;
	
	RockManager(SceneManager* mng, Transform* trams);
	~RockManager();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	Transform* mPlayerTransform;
	ResourceManager* mResourceManager;
	SceneManager* mSceneManager;

	std::map<IntVector3, std::vector<Rock*>> mMapRocks;

	Rock* CreateRandom(IntVector3 mapPos);
};

