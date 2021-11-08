//#pragma once
#include <chrono>
#include "DemoScene.h"
class SceneBase;
class Fader;
class Camera;

// シーン管理用
enum class SCENE_ID
{
	NONE
	, TITLE
};

class SceneManager
{

public:

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Release(void);

	void ChangeScene(SCENE_ID nextId, bool isFading);

	// デルタタイムの取得
	float GetDeltaTime(void);

	Camera* GetCamera(void);

private:

	SCENE_ID mSceneID;
	SCENE_ID mWaitSceneID;

	SceneBase* mScene;
	Fader* mFader;
	Camera* mCamera;

	bool mIsSceneChanging;

	// デルタタイム
	std::chrono::system_clock::time_point mTickCount;
	float mDeltaTime;

	void DoChangeScene(void);

};