#include "Trail.h"
#include "HomingShot.h"
#include <DxLib.h>

constexpr size_t history_limit = 10;

Trail::Trail(HomingShot& owner):owner_(owner)
{
}

void Trail::Update()
{
	// 今のオーナーの座標
	history_.push_front(owner_.pos);
	if (history_.size() > history_limit)
	{
		history_.pop_back();
	}
}

void Trail::Draw()
{
	auto lastPos = owner_.pos;
	float thickness = 20.0f;
	for (const auto& pos : history_)
	{
		DrawLineAA(lastPos.x, lastPos.y, pos.x, pos.y, 0xffff, thickness);
		thickness *= 0.8f;
		lastPos = pos;
	}

}

void Trail::Clrear()
{
	history_.clear();
}
