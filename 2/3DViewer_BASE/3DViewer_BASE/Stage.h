#pragma once
#include <DxLib.h>

class SceneManager;
class Stage
{
public:
	Stage(SceneManager* mng);

	void Init();
	void Update();
	void Draw();
	void Relese();

private:
	SceneManager* mSceneMng_;

	int modelHandle_;
	VECTOR modelPos_;
};

