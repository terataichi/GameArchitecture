#pragma once
#include <DxLib.h>

class SceneManager;
class Unit;
class RollBall
{
public:
	RollBall(SceneManager* mng,Unit* unit);
	~RollBall();
	void Init();
	void Update();
	void Draw();
	void Relese();

	VECTOR GetPos();
	float GetAngle();
private:
	SceneManager* mSceneMng_;
	Unit* unit_;

	VECTOR pos_;
	float angle_;
	float deltaStep_;


};

