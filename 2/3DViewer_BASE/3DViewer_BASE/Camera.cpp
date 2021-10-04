#include "Camera.h"
#include "AsoUtility.h"
#include "Unit.h"

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
	pos_ = { 0.0f,HEIGHT,-DIS_TARGET_TO_CAMERA };
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
	//moveAngle(CheckHitKey(KEY_INPUT_NUMPAD0), angle_.z, -ang);
	//moveAngle(CheckHitKey(KEY_INPUT_NUMPAD1), angle_.z, ang);
	moveAngle(CheckHitKey(KEY_INPUT_RIGHT), angle_.y, ang);
	moveAngle(CheckHitKey(KEY_INPUT_LEFT), angle_.y, -ang);

	//pos_ = target_;
}

void Camera::SetBeforeDraw()
{
	if (unit_ == nullptr)
	{
		SetCameraPositionAndAngle(
			pos_, angle_.x, angle_.y, angle_.z
		);
	}
	else
	{
		// 注視店の更新

		target_ = unit_->GetPos();

		float cameraDirX = sinf(angle_.y);
		float cameraDirZ = cosf(angle_.y);

		VECTOR cameraDIR = VNorm({ cameraDirX ,0.0f,cameraDirZ });
		VECTOR targetPow = VScale(cameraDIR, 300);

		target_ = VAdd(target_,targetPow );


		// まずカメラへの方向計算
		float dirX = sinf(angle_.y + AsoUtility::Deg2RadF(180.0f));
		float dirZ = cosf(angle_.y + AsoUtility::Deg2RadF(180.0f));
		// 単位ベクトルに変換
		VECTOR dir = VNorm({ dirX ,0.0f,dirZ });
		// 大きさとかけて座標を出す
		VECTOR movePow = VScale(dir, DIS_TARGET_TO_CAMERA);

		pos_.x = target_.x + movePow.x;
		pos_.z = target_.z + movePow.z;
		pos_.y = HEIGHT;

		

		SetCameraPositionAndTargetAndUpVec(pos_, target_, VECTOR{ 0.0f,1.0f,0.0f });
	}
	//SetCameraPositionAndTarget_UpVecY(pos_, target_);

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

void Camera::SetUnit(Unit* unit)
{
	unit_ = unit;
}

void Camera::SetTarget(VECTOR& pos)
{
	target_ = pos;
}
