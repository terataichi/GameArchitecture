#pragma once
#include "Geometry.h"
#include "Trail.h"

struct Bullet {
	Position2 pos;//座標
	Vector2 vel;//速度
	bool isActive = false;//生きてるか〜？
};

struct HomingShot:public Bullet
{
	HomingShot();
	Trail trail;
};

