#include <DxLib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cassert>
#include "Geometry.h"

Position2 GetMousePos()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	return { (float)mx,(float)my };
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//DxLib::SetGraphMode(500, 500, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(L"1916025_寺崎大智");
	if (DxLib_Init())
	{
		return false;
	}

	int handle = LoadGraph(L"image/arrow2.png", true);
	Vector2 offset{16,35 };

	
	constexpr size_t picture_num = 5;
	int graphH[6] = {};
	for (int i = 0; i < picture_num; ++i){

		std::wostringstream oss;
		oss << L"image/Individual Sprites/adventurer-attack1-";
		oss << std::setw(2);
		oss << std::setfill(L'0');
		oss << i <<".png";

		graphH[i] = LoadGraph(oss.str().c_str());
		assert(graphH[i] != -1);
	}

	Rect rect = { 50.0f,50.0f,75,75 };

	int a[10] = {};
	int count_ = 0;
	a[5]++;
	a[7]++;
	constexpr size_t frames_per_pict = 5;
	unsigned int frameNo = 0;
	int frameForAngle = 0;
	bool isReverse = false;
	int lastMouseInput;
	while (DxLib::ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		DxLib::SetDrawScreen(DX_SCREEN_BACK);
		DxLib::ClsDrawScreen();
		
	

		auto mPos = GetMousePos();

		Vector2 dir = mPos - rect.Center();

		auto mouseinput = GetMouseInput();

		if ((mouseinput & MOUSE_INPUT_LEFT) && 
			!(lastMouseInput & MOUSE_INPUT_LEFT))
		{
			isReverse = !isReverse;
		}

		lastMouseInput = mouseinput;

		constexpr size_t blockSize = 16;
		constexpr size_t width = 1000;

		auto count = width / blockSize;

		float weidth = 800.0f / static_cast<float>(width);

		int base_y = 240;
		int sine_amp = 30;

		Vector2 lastDelta90Vectors[2] = { Vector2::ZERO,Vector2::ZERO };
		Position2 lastPos = Vector2::ZERO;
		int x = 0;
		int y = base_y;
		Position2 currentPos(x,y);
		for (int i = 0; i < count; i++)
		{
			float nextX = i * blockSize;
			float nextY = sine_amp * sinf((nextX * 0.5f + frameForAngle) / 180.0f * DX_PI_F);
			float offset = 50.0f * sinf((100 + frameForAngle) / 180.0f * DX_PI_F);
			//DrawLineAA(x,y,nextX,nextY,0xff,5.0f);

			// 次までのベクトル
			auto deltaVec = Vector2(blockSize, nextY).Normalized() * blockSize;

			auto nextPos = currentPos + deltaVec;


			auto middleVec1 = deltaVec.Rotated90();
			auto middleVecR = deltaVec;
			if (!(lastDelta90Vectors[0] == Vector2::ZERO))
			{
				middleVecR = (middleVec1 + lastDelta90Vectors[0]).Normalized() * blockSize;
			}
			auto middleVecL = lastDelta90Vectors[0];
			if (!(lastDelta90Vectors[1] == Vector2::ZERO))
			{
				middleVecL = (middleVecL + lastDelta90Vectors[1]).Normalized() * blockSize;
			}
			lastDelta90Vectors[1] = lastDelta90Vectors[0];
			lastDelta90Vectors[0] = deltaVec.Rotated90();

			auto middlePosL = currentPos + middleVecL;
			auto middlePosR = nextPos + middleVecR;

			auto rightPos = nextPos + deltaVec.Rotated90();
			auto leftPos = currentPos + deltaVec.Rotated90();

			if (lastPos == Vector2::ZERO)
			{
				auto middlePosL = currentPos + middleVecL;
				auto middlePosR = nextPos + middleVecR;

				DrawRectModiGraph(currentPos.x, currentPos.y + offset,
					nextPos.x, nextPos.y + offset,
					middlePosR.x, middlePosR.y + offset,
					middlePosL.x, middlePosL.y + offset,
					i* blockSize* weidth, 0,
					blockSize, 64,
					handle, true);
			}
			else
			{
				auto middlePosL = lastPos + middleVecL;
				auto middlePosR = currentPos + middleVecR;

				DrawRectModiGraph(lastPos.x, lastPos.y + offset,
					currentPos.x, currentPos.y + offset,
					middlePosR.x, middlePosR.y + offset,
					middlePosL.x, middlePosL.y + offset,
					i * blockSize * weidth, 0,
					blockSize, 64,
					handle, true);
			}

			lastPos = currentPos;
			currentPos = nextPos;
		}
		// 基準が右向き

		if (CheckHitKey(KEY_INPUT_A))
		{
			rect.pos.x -= 10;
		}
		if (CheckHitKey(KEY_INPUT_W))
		{
			rect.pos.y -= 10;
		}		
		if (CheckHitKey(KEY_INPUT_S))
		{
			rect.pos.y += 10;
		}		
		if (CheckHitKey(KEY_INPUT_D))
		{
			rect.pos.x += 10;
		}


		float angle = atan2f(dir.y, dir.x);

		int image = frameNo / frames_per_pict;
		int flipX = 16;
		if (isReverse)
		{
			int w, h;
			GetGraphSize(graphH[image], &w, &h);
			flipX = w - flipX;
		}
		DrawRotaGraph2(rect.Center().x, rect.Center().y,
			flipX, 35,											// 中心点
			2.0f, angle, graphH[image], true,isReverse);

		DxLib::ScreenFlip();

		frameNo = (frameNo + 1) % (frames_per_pict * picture_num);
		frameForAngle = (frameForAngle + 1) % 720;
	}
	DxLib_End();
	return 0;
}