#pragma once
#include <DxLib.h>
class Unit;
class Camera
{
public:
	// カメラの高さ
	static constexpr float HEIGHT = 200.0f;
	// 廚視点からカメラまでのXZ距離
	static constexpr float DIS_TARGET_TO_CAMERA = 500.0f;

	Camera();
	~Camera();

	void Init();
	void Update();
	void SetBeforeDraw();		// カメラの設定　setDrawScreen の後に呼ばないといけない
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

