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
	DxLib::SetWindowText(L"1916025_éõçËëÂíq");
	if (DxLib_Init())
	{
		return false;
	}

	int handle = LoadGraph(L"image/Assets.png", true);
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

		constexpr size_t blockSize = 32;
		auto count = 720 / blockSize;
		int base_y = 240;
		int sine_amp = 100.0f;
		int x = 0;
		int y = base_y;
		for (int i = 0; i < count; i++)
		{
			float nextX = i * blockSize;
			float nextY = base_y + sine_amp * sinf((nextX * 0.5f + frameForAngle) / 180.0f * DX_PI_F);
			//DrawLineAA(x,y,nextX,nextY,0xff,5.0f);
			DrawRectModiGraph(x, y,
				nextX, nextY,
				nextX, nextY + blockSize,
				x, y + blockSize,
				48,0,
				16,16,
				handle, true);

			x = nextX;
			y = nextY;
		}
		// äÓèÄÇ™âEå¸Ç´

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
			flipX, 35,											// íÜêSì_
			2.0f, angle, graphH[image], true,isReverse);

		DxLib::ScreenFlip();

		frameNo = (frameNo + 1) % (frames_per_pict * picture_num);
		frameForAngle = (frameForAngle + 1) % 720;
	}
	DxLib_End();
	return 0;
}