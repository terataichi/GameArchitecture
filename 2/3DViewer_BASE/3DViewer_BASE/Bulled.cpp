#include "Bulled.h"

Bulled::Bulled(VECTOR pos,VECTOR vel)
	:pos_(pos),vel_(vel)
{
	alive_ = true;
	time_ = 0.0f;
}

Bulled::~Bulled()
{
}

void Bulled::Init()
{
}

void Bulled::Update(float delta)
{

	time_ += delta;

	if (time_ > 2.0f)
	{
		alive_ = false;
	}

	pos_ = VAdd(pos_,vel_);
}

void Bulled::Draw()
{
	DrawSphere3D(pos_, 10.0f, 32, 0xfff, 0xfff, true);
}

void Bulled::Relese()
{
}

VECTOR Bulled::GetPos()
{
	return pos_;
}

bool Bulled::Alive()
{
	return alive_;
}
