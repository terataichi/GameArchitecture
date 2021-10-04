#pragma once
#include <DxLib.h>

class SceneManager;
class Enemy
{
public:
	static constexpr float SPEED_ROT = 5.0f;
	static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

	Enemy(SceneManager* mng);
	~Enemy();
	void Init();
	void Update();
	void Draw();
	void Relese();

	VECTOR GetPos();
	VECTOR GetAngle();

private:
	SceneManager* mSceneMng_;

	int modelHandle_;
	VECTOR pos_;
	VECTOR angle_;
	VECTOR localAngle_;
	// �A�j���[�V�����p�̃n���h��ID
	int animWalkHandle_;
	// �A�j���[�V�����̂����Đ����ԕێ�
	float animTime_;

	// �A�j���[�V�����p�̍X�V�X�e�b�v
	float animStep_;

	float animSpeed_;
	float rotRad_;
};

