#include<DxLib.h>
#include<cmath>
#include <memory>
#include <list>
#include"Geometry.h"
#include "HomingShot.h"

///�����蔻��֐�
///@param posA A�̍��W
///@param radiusA A�̔��a
///@param posB B�̍��W
///@param radiusB B�̔��a
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//�����蔻����������Ă�������
	return false;
}

#define MAX_BULLED 16

// �o���b�g�̂�������

namespace {
	// ����
	//HomingShot
	// .cpp

	// .h
	//bulled.h��include
	//Trail
	// .cpp
	// div=1.0f/static_cast<float>(histry_.size())
	// .h
	// 	   int handle_
	// SetHandle(int handle)
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("1916025_�����q");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	std::list<Position2>;

	//�w�i�p
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

	//�e�̔��a
	float bulletRadius = 5.0f;

	//���@�̔��a
	float playerRadius = 10.0f;

	//�K����256���炢����Ƃ�
	Bullet bullets[256];

	// �v���C���[�̃z�[�~���O�e
	HomingShot homingShots[MAX_BULLED] = {};

	for (auto shot : homingShots)
	{
		// shot.SetHandle(arrowH);
	}

	Position2 enemypos(320,25);//�G���W
	Position2 playerpos(320, 400);//���@���W

	unsigned int frame = 0;//�t���[���Ǘ��p

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

		//�w�i
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);

		//�v���C���[
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
			// �z�[�~���O�ʔ���
			DrawString(100, 100, "����", 0xf);
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
		// �v���C���[�̃z�[�~���O�e�̏���
		int i = 0;
		for (auto& hshot : homingShots)
		{
			// �A�N�e�B�u���ǂ���
			if (hshot.isActive)
			{
				if (frame % 5 == 0)
				{
					hshot.trail.Update();
				}
				hshot.pos += hshot.vel;
				hshot.trail.Draw();

				// �ӊO�Əo�����ǂ������P
				// �ȈՔ�
				//for (float i = 1; i < 5; i++)
				//{
				//	auto tailPos = hshot.pos - hshot.vel * 2.0f * i;
				//	auto thickness = 6.0f - i;
				//	DrawLineAA(hshot.pos.x, hshot.pos.y,
				//		tailPos.x, tailPos.y, 0xff0000, thickness * 4.0f);
				//}
				//hshot.vel = (hshot.vel + (enemypos - hshot.pos).Normalized()).Normalized() * homingShotSpeed;

				// �G�ւ̃x�N�g���A����э��̑��x�x�N�g���𐳋K��
				auto nVelocity = hshot.vel.Normalized();
				auto nToEnemy = (enemypos - hshot.pos).Normalized();
				auto dot = Dot(nVelocity, nToEnemy);							// cos�����o��
				auto angle = acos(dot);											// �p�x���o��(0)
				angle = std::fminf(angle, DX_PI_F / 24.0f);
				auto sgin = Cross(nVelocity, nToEnemy) > 0.0f ? 1.0f : -1.0f;	// 
				angle = angle * sgin + atan2f(hshot.vel.y, hshot.vel.x);
				hshot.vel = hshot.vel.Normalized() + Vector2(cos(angle), sin(angle)) * homingShotSpeed;

				DrawCircleAA(hshot.pos.x, hshot.pos.y,
					8.0f, 16, 0xff0000, true);

				// �G�ɓ�������
				if ((enemypos - hshot.pos).SQMagnitude() < 30.0f * 30.0f)
				{
					hshot.isActive = false;
					hshot.trail.Clrear();
				}
				//�e���E��
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
			//���@�̖{��(�����蔻��)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		// �p�t�H�[�}���X�v��
		int dCallCnt = GetDrawCallCount();
		float fps = GetFPS();

		DrawFormatString(11, 51, 0x000000, "DrawCall=%d", dCallCnt);
		DrawFormatString(10, 50, 0xff0000, "DrawCall=%d", dCallCnt);
		DrawFormatString(11, 101, 0x000000, "Fps=%f", fps);
		DrawFormatString(10, 100, 0xff0000, "Fps=%f", fps);

		//�e����
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

		//�e�̍X�V����ѕ\��
		for (auto& b : bullets) {
			if (!b.isActive) {
				continue;
			}

			//�e�̌��ݍ��W�ɒe�̌��ݑ��x�����Z���Ă�������
			
			float angle = 0.0f;
			//�e�̊p�x��atan2�Ōv�Z���Ă��������Bangle�ɒl������񂾂�I�D
			
			DrawRotaGraph(b.pos.x, b.pos.y,1.0f,angle, bulletH, true);
			
			if (isDebugMode) {
				//�e�̖{��(�����蔻��)
				DrawCircle(b.pos.x, b.pos.y, bulletRadius, 0x0000ff, false, 3);
			}
			//�e���E��
			if (b.pos.x + 16 < 0 || b.pos.x - 16 > 640 ||
				b.pos.y + 24 < 0 || b.pos.y - 24 > 480) {
				b.isActive = false;
			}

			//������I
			//����IsHit�͎����������Ă܂���B�����ŏ����Ă��������B
			if (IsHit(b.pos, bulletRadius, playerpos, playerRadius)) {
				//�������������������Ă��������B
			}
		}

		//�G�̕\��
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//�G�̖{��(�����蔻��)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}
		++frame;
		ScreenFlip();

		std::copy(std::begin(keystate), std::end(keystate), std::begin(lastKeyState));
	}

	DxLib_End();

	return 0;
}