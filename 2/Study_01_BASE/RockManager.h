#pragma once
#include <map>
#include <vector> 
#include "Transform.h"
#include "IntVector3.h"

class SceneManager;
class ResourceManager;
class Player;
class Rock;

class RockManager
{
public:
	RockManager(SceneManager* sceneManager,Player* player);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void RandomRockInstance(void);
	Rock* CreateRock(IntVector3 mPos);
private:
	SceneManager* mSceneManager;
	Player* mPlayer;
	ResourceManager* mResourceManager;

	std::map < IntVector3, std::vector<Rock*>> mMapRocks;
};

