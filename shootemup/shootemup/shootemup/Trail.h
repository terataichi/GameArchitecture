#pragma once
#include "Geometry.h"
#include <list>

struct HomingShot;

class Trail
{

private:
	HomingShot& owner_;
	std::list<Position2> history_;		// À•W‚Ì—š—ğ
	int handle_;
public:
	Trail(HomingShot& owner);
	void SetHandle(int handle);
	void Update();
	void Draw();
	void Clrear();
};

