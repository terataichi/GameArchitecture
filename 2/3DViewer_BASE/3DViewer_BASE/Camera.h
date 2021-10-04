#pragma once
#include <DxLib.h>
class Unit;
class Camera
{
public:
	// �J�����̍���
	static constexpr float HEIGHT = 200.0f;
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
	void SetUnit(Unit* unit);
	void SetTarget(VECTOR& pos);
private:

	VECTOR pos_;
	VECTOR angle_;
	VECTOR target_;
	Unit* unit_;
};

