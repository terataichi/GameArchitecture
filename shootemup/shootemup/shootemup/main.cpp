#include<DxLib.h>
#include<cmath>
#include <memory>
#include <list>
#include"Geometry.h"
#include "HomingShot.h"

///当たり判定関数
///@param posA Aの座標
///@param radiusA Aの半径
///@param posB Bの座標
///@param radiusB Bの半径
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//当たり判定を実装してください
	return false;
}

#define MAX_BULLED 16

// バレットのｈをつくる

namespace {
	// メモ
	//HomingShot
	// .cpp

	// .h
	//bulled.hをinclude
	//Trail
	// .cpp
	// div=1.0f/static_cast<float>(histry_.size())
	// .h
	// 	   int handle_
	// SetHandle(int handle)
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("1916025_寺崎大智");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	std::list<Position2>;

	//背景用
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	auto bulletH=LoadGraph("img/bullet.png");
	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	int arrowH = LoadGraph("img/arrow2.png");

	//弾の半径
	float bulletRadius = 5.0f;

	//自機の半径
	float playerRadius = 10.0f;

	//適当に256個くらい作っとく
	Bullet bullets[256];

	// プレイヤーのホーミング弾
	HomingShot homingShots[MAX_BULLED] = {};

	for (auto shot : homingShots)
	{
		// shot.SetHandle(arrowH);
	}

	Position2 enemypos(320,25);//敵座標
	Position2 playerpos(320, 400);//自機座標

	unsigned int frame = 0;//フレーム管理用

	char keystate[256]{};
	char lastKeyState[256]{};

	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;
	constexpr float homingShotSpeed = 10.0f;

	bool isRightHoming = false;

	while (ProcessMessage() == 0 &&!CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//背景
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);

		//プレイヤー
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}
		if (keystate[KEY_INPUT_Z] && !lastKeyState[KEY_INPUT_Z])
		{
			int count = 0;
			// ホーミング玉発射
			DrawString(100, 100, "発射", 0xf);
			for (auto& hshot : homingShots)
			{
				if (!hshot.isActive)
				{
					hshot.isActive = true;
					hshot.pos = playerpos;

					hshot.vel = { count == 0 ? homingShotSpeed : -homingShotSpeed, 10.0f };
					hshot.vel.Normalize();
					hshot.vel *= homingShotSpeed;
					isRightHoming = !isRightHoming;
					if (++count > 2)
					{
						break;
					}
				}
			}
		}
		// プレイヤーのホーミング弾の処理
		int i = 0;
		for (auto& hshot : homingShots)
		{
			// アクティブかどうか
			if (hshot.isActive)
			{
				if (frame % 5 == 0)
				{
					hshot.trail.Update();
				}
				hshot.pos += hshot.vel;
				hshot.trail.Draw();

				// 意外と出来が良かった訓
				// 簡易版
				//for (float i = 1; i < 5; i++)
				//{
				//	auto tailPos = hshot.pos - hshot.vel * 2.0f * i;
				//	auto thickness = 6.0f - i;
				//	DrawLineAA(hshot.pos.x, hshot.pos.y,
				//		tailPos.x, tailPos.y, 0xff0000, thickness * 4.0f);
				//}
				//hshot.vel = (hshot.vel + (enemypos - hshot.pos).Normalized()).Normalized() * homingShotSpeed;

				// 敵へのベクトル、および今の速度ベクトルを正規化
				auto nVelocity = hshot.vel.Normalized();
				auto nToEnemy = (enemypos - hshot.pos).Normalized();
				auto dot = Dot(nVelocity, nToEnemy);							// cosΘが出る
				auto angle = acos(dot);											// 角度が出る(0)
				angle = std::fminf(angle, DX_PI_F / 24.0f);
				auto sgin = Cross(nVelocity, nToEnemy) > 0.0f ? 1.0f : -1.0f;	// 
				angle = angle * sgin + atan2f(hshot.vel.y, hshot.vel.x);
				hshot.vel = hshot.vel.Normalized() + Vector2(cos(angle), sin(angle)) * homingShotSpeed;

				DrawCircleAA(hshot.pos.x, hshot.pos.y,
					8.0f, 16, 0xff0000, true);

				// 敵に当たった
				if ((enemypos - hshot.pos).SQMagnitude() < 30.0f * 30.0f)
				{
					hshot.isActive = false;
					hshot.trail.Clrear();
				}
				//弾を殺す
				if (hshot.pos.x + 16 < 0 || hshot.pos.x - 16 > 640 ||
					hshot.pos.y + 24 < 0 || hshot.pos.y - 24 > 480) {
					hshot.isActive = false;
				}
			}
			i++;
		}

		DrawCircleAA(enemypos.x, enemypos.y, 50.0f, 16, 0x00ff00, false);

		int pidx = (frame / 4 % 2) * 5 + 3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//自機の本体(当たり判定)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		// パフォーマンス計測
		int dCallCnt = GetDrawCallCount();
		float fps = GetFPS();

		DrawFormatString(11, 51, 0x000000, "DrawCall=%d", dCallCnt);
		DrawFormatString(10, 50, 0xff0000, "DrawCall=%d", dCallCnt);
		DrawFormatString(11, 101, 0x000000, "Fps=%f", fps);
		DrawFormatString(10, 100, 0xff0000, "Fps=%f", fps);

		//弾発射
		if (frame % 12 == 0) {
			for (auto& b : bullets) {
				if (!b.isActive) {
					//b.pos = enemypos;
					//b.vel = Vector2(0, 5);
					//b.isActive = true;
					break;
				}
			}
		}

		//弾の更新および表示
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//弾の現在座標に弾の現在速度を加算してください
			
			float angle = 0.0f;
			//弾の角度をatan2で計算してください。angleに値を入れるんだよオゥ
			
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//弾の本体(当たり判定)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}
			//弾を殺す
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//あたり！
			//↓のIsHitは実装を書いてません。自分で書いてください。
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//当たった反応を書いてください。
			}
		}

		//敵の表示
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//敵の本体(当たり判定)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();

		std::copy(std::begin(keystate), std::end(keystate), std::begin(lastKeyState));
	}

	DxLib_End();

	return 0;
}