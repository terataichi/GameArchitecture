#include "Trail.h"
#include "HomingShot.h"
#include <DxLib.h>

constexpr size_t history_limit = 10;

Trail::Trail(HomingShot& owner):owner_(owner)
{
}

void Trail::SetHandle(int handle)
{
	handle_ = handle;
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
	auto div = 1.0f / static_cast<float>(history_.size());
	auto u = 0.0f;
	for (const auto& pos : history_)
	{
		if (lastPos == pos)continue;
		//DrawLineAA(lastPos.x, lastPos.y, pos.x, pos.y, 0xffff, thickness);
		thickness *= 0.8f;

		auto v = pos - lastPos;
		v.Normalize();
		v = Vector2{ -v.y,v.x };
		auto p1 = lastPos + v * 16;
		auto p2 = pos + v * 16;
		auto p3 = pos - v * 16;
		auto p4 = lastPos - v * 16;

		// DrawPolygonIndexed2D();
		DrawRectModiGraph(p1.x, p1.y,p2.x,p2.y, p3.x, p3.y, p4.x, p4.y,
			u * 256,0,div * 256 , 64 , handle_,true);

		lastPos = pos;
		u += div;
	}

}

void Trail::Clrear()
{
	history_.clear();
}
