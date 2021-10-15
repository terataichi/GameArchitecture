#include "Enemy.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "Camera.h"
#include "Unit.h"

#include <cmath>

Enemy::Enemy(SceneManager* mng,	Unit* unit) :mSceneMng_(mng),unit_(unit)
{
	Init();
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	animSpeed_ = 60.0f;

	modelHandle_ = MV1LoadModel("Model/Capoeira.mv1");
	pos_ = { 0.0f,0.0f,500.0f };
	angle_ = { 0.0f,AsoUtility::Deg2RadF(180.0f),0.0f };
	localAngle_ = { 0.0f,AsoUtility::Deg2RadF(180.0f),0.0f };

	MV1SetPosition(modelHandle_, pos_);

	// �p�x�ݒ�
	auto angle = angle_;
	angle.x += localAngle_.x;
	angle.y += localAngle_.y;
	angle.z += localAngle_.z;

	MV1SetRotationXYZ(modelHandle_, angle);

	animWalkHandle_ = MV1AttachAnim(modelHandle_, 0);

	// �A�j���[�V�����Đ����Ԏ擾
	animTime_ = MV1GetAttachAnimTotalTime(modelHandle_, animWalkHandle_);

	animStep_ = 0.0f;

	// ���f���ɃA�j���[�V������K�p����
	MV1SetAttachAnimTime(modelHandle_, animWalkHandle_, animStep_);
}

void Enemy::Update()
{
	// �o�ߎ��Ԏ擾
	float delta = mSceneMng_->GetDeltaTime();

	animStep_ += animSpeed_ * delta;

	if (animStep_ > animTime_)
	{
		animStep_ = 0;
	}

	MV1SetAttachAnimTime(modelHandle_, animWalkHandle_, animStep_);

	// �Q�_�Ԃ̋��������߂�
	auto diff = VSub(unit_->GetPos(), pos_);
	auto length = VSize(diff);

	if (200.0f > length)
	{

		float angle = std::abs(std::atan2f(diff.x, diff.z));

		auto degAngle = AsoUtility::Rad2Deg(angle_.y);

		auto min = AsoUtility::Deg2RadF(degAngle - 30.0f);
		auto max = AsoUtility::Deg2RadF(degAngle + 30.0f);

		if ((AsoUtility::RadIn2PI(min) < angle) &&
			(AsoUtility::RadIn2PI(max) > angle))
		{
			MV1SetMaterialDifColor(modelHandle_, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			MV1SetMaterialDifColor(modelHandle_, 0, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
		}

	}
	else
	{
		MV1SetMaterialDifColor(modelHandle_, 0, GetColorF(0.0f, 1.0f, 0.0f, 1.0f));
	}
	

	// �p�x�ݒ�
	auto angle = angle_;
	angle.x += localAngle_.x;
	angle.y += localAngle_.y;
	angle.z += localAngle_.z;
	MV1SetRotationXYZ(modelHandle_, angle);
	MV1SetPosition(modelHandle_, pos_);
}

void Enemy::Draw()
{
	// ���f���`��
	MV1DrawModel(modelHandle_);

	VECTOR eyeCenter = VECTOR{std::sinf(angle_.y),0.0f,std::cosf(angle_.y)};
	eyeCenter = VScale(eyeCenter,200.0f);

	VECTOR eyeLeft = VECTOR{ std::sinf(angle_.y + AsoUtility::Deg2RadF(-30.0f)),0.0f,std::cosf(angle_.y +AsoUtility::Deg2RadF(-30.0f)) };
	eyeLeft = VScale(eyeLeft, 200.0f);

	VECTOR eyeRight = VECTOR{ std::sinf(angle_.y + AsoUtility::Deg2RadF(30.0f)),0.0f,std::cosf(angle_.y + AsoUtility::Deg2RadF(30.0f)) };
	eyeRight = VScale(eyeRight, 200.0f);


	DrawTriangle3D(pos_, VAdd(pos_,eyeCenter), VAdd(pos_, eyeLeft), 0xffff, false);
	DrawTriangle3D(pos_, VAdd(pos_, eyeCenter), VAdd(pos_, eyeRight), 0xffff, false);

}

void Enemy::Relese()
{
}

VECTOR Enemy::GetPos()
{
	return pos_;
}

VECTOR Enemy::GetAngle()
{
	return angle_;
}
