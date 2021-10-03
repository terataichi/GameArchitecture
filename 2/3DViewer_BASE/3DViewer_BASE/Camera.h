#pragma once
#include <DxLib.h>
class Camera
{
public:
	// �J�����̍���
	static constexpr float HIGHT = 200.0f;
	// �C���_����J�����܂ł�XZ����
	static constexpr float DIS_TARGET_TO_CAMERA = 500.0f;

	Camera();
	~Camera();

	void Init();
	void Update();
	void SetBeforeDraw();		// �J�����̐ݒ�@setDrawScreen �̌�ɌĂ΂Ȃ��Ƃ����Ȃ�
	void Draw();
	void Relese();

	const VECTOR& GetPos();
	const VECTOR& GetAngle();
	void SetTardet(VECTOR& pos);
private:

	VECTOR pos_;
	VECTOR angle_;
	VECTOR target_;
};

