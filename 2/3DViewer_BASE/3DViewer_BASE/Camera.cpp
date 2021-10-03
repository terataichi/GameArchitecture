#include "Camera.h"
#include "AsoUtility.h"
//#include <DxLib.h>

Camera::Camera()
{
	Init();
}

Camera::~Camera()
{
}

void Camera::Init()
{
	pos_ = { 0.0f,200.0f,-500.0f };
	angle_ = { static_cast<float>(AsoUtility::Deg2RadD(30.0)),0.0f,0.0f };
	SetCameraPositionAndAngle(
		pos_, angle_.x, angle_.y, angle_.z
	);
}

void Camera::Update()
{
	// キーボードでカメラを動かしてみる

	//auto move = [](bool check,VECTOR& pos,VECTOR vel)
	//{
	//	if (check)
	//	{
	//		pos.x += vel.x;
	//		pos.y += vel.y;
	//		pos.z += vel.z;
	//	}
	//};
	//float vel = 3.0f;

	//move(CheckHitKey(KEY_INPUT_W), pos_, { sinf(angle_.y + AsoUtility::Deg2RadD(0.0f)) * vel ,0.0f,cosf(angle_.y + AsoUtility::Deg2RadD(0.0f)) * vel });
	//move(CheckHitKey(KEY_INPUT_S), pos_, { sinf(angle_.y + AsoUtility::Deg2RadD(180.0f)) * vel,0.0f,cosf(angle_.y + AsoUtility::Deg2RadD(180.0f)) * vel });
	//move(CheckHitKey(KEY_INPUT_A), pos_, { sinf(angle_.y + AsoUtility::Deg2RadD(-90.0f)) * vel ,0.0f,cosf(angle_.y + AsoUtility::Deg2RadD(-90.0f)) * vel });
	//move(CheckHitKey(KEY_INPUT_D), pos_, { sinf(angle_.y + AsoUtility::Deg2RadD(90.0f)) * vel ,0.0f,cosf(angle_.y + AsoUtility::Deg2RadD(90.0f)) * vel });
	//move(CheckHitKey(KEY_INPUT_Q), pos_, { 0.0f,vel,0.0f });
	//move(CheckHitKey(KEY_INPUT_E), pos_, { 0.0f,-vel,0.0f });

	// キーボードでカメラ角度を変えてみる
	float ang = AsoUtility::Deg2RadD(1);

	auto moveAngle = [](bool check, float& angle, float vel)
	{
		if (check)
		{
			angle += vel;
		}
	};
	moveAngle(CheckHitKey(KEY_INPUT_UP), angle_.x, ang);
	moveAngle(CheckHitKey(KEY_INPUT_DOWN), angle_.x, -ang);
	moveAngle(CheckHitKey(KEY_INPUT_NUMPAD0), angle_.z, -ang);
	moveAngle(CheckHitKey(KEY_INPUT_NUMPAD1), angle_.z, ang);
	moveAngle(CheckHitKey(KEY_INPUT_RIGHT), angle_.y, ang);
	moveAngle(CheckHitKey(KEY_INPUT_LEFT), angle_.y, -ang);

	//pos_ = target_;
}

void Camera::SetBeforeDraw()
{

	//SetCameraPositionAndTarget_UpVecY(pos_, target_);
	SetCameraPositionAndTargetAndUpVec(pos_, target_, VECTOR{ 0.0f,0.0f,0.0f });
	//SetCameraPositionAndAngle(
	//	pos_, angle_.x, angle_.y, angle_.z
	//);
}

void Camera::Draw()
{
}

void Camera::Relese()
{
}

const VECTOR& Camera::GetPos()
{
	return pos_;
}

const VECTOR& Camera::GetAngle()
{
	return angle_;
}

void Camera::SetTardet(VECTOR& pos)
{
	target_ = pos;
}
