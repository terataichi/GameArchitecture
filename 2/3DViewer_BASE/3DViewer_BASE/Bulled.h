#pragma once
#include <DxLib.h>

class Bulled
{
public:
	Bulled(VECTOR pos,VECTOR vel);
	~Bulled();
	void Init();
	void Update(float delta);
	void Draw();
	void Relese();

	VECTOR GetPos();
	bool Alive();
private:
	VECTOR pos_;
	VECTOR vel_;

	float time_;
	bool alive_;
};

