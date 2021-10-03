#include "Unit.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "Camera.h"

Unit::Unit(SceneManager* mng) :mSceneMng_(mng)
{
	Init();
}

Unit::~Unit()
{
}

void Unit::Init()
{
	animSpeed_ = 60.0f;

	modelHandle_ = MV1LoadModel("Model/Capoeira.mv1");
	pos_ = { 0.0f,0.0f,0.0f };
	angle_ = {};
	localAngle_ = { 0.0f,AsoUtility::Deg2RadF(180.0f),0.0f };

	MV1SetPosition(modelHandle_, pos_);

	// 角度設定
	auto angle = angle_;
	angle.x += localAngle_.x;
	angle.y += localAngle_.y;
	angle.z += localAngle_.z;

	MV1SetRotationXYZ(modelHandle_, angle);

	animWalkHandle_ = MV1AttachAnim(modelHandle_, 0);

	// アニメーション再生時間取得
	animTime_ = MV1GetAttachAnimTotalTime(modelHandle_, animWalkHandle_);

	animStep_ = 0.0f;

	// モデルにアニメーションを適用する
	MV1SetAttachAnimTime(modelHandle_, animWalkHandle_, animStep_);
}

void Unit::Update()
{
	// 経過時間取得
	float delta = mSceneMng_->GetDeltaTime();

	animStep_ += animSpeed_ * delta;

	if (animStep_ > animTime_)
	{
		animStep_ = 0;
	}

	MV1SetAttachAnimTime(modelHandle_, animWalkHandle_, animStep_);
	auto camera = mSceneMng_->GetCamera();
	auto cameraAngle = camera->GetAngle();
	auto move = [&](bool check, VECTOR& pos, VECTOR vel,float rad)
	{
		if (check)
		{
			pos.x += vel.x;
			pos.y += vel.y;
			pos.z += vel.z;
			rotRad_ = rad;

			float radY = angle_.y;
			float moveRadY = rotRad_ + cameraAngle.y;
			// くるくる問題
			moveRadY = AsoUtility::RadIn2PI(moveRadY);
			radY = AsoUtility::RadIn2PI(radY);

			// 回転量の少ないほうの回転方向を取得する
			float aroundDir = AsoUtility::DirNearAroundRad(radY, moveRadY);

			// ぶるぶる問題
			if (abs(radY - moveRadY)  < SPEED_ROT_RAD * 2.0f)
			{
				angle_.y = moveRadY;
				return;
			}
			angle_.y += SPEED_ROT_RAD * aroundDir;
		}
	};
	float vel = 3.0f;



	move(CheckHitKey(KEY_INPUT_W), pos_, { sinf(cameraAngle.y + AsoUtility::Deg2RadD(0.0f)) * vel,0.0f,cosf(cameraAngle.y + AsoUtility::Deg2RadD(0.0f)) * vel},
		cameraAngle.y + AsoUtility::Deg2RadF(0.0f));
	move(CheckHitKey(KEY_INPUT_S), pos_, 
		{ sinf(cameraAngle.y + AsoUtility::Deg2RadD(180.0f)) * vel,0.0f,cosf(cameraAngle.y + AsoUtility::Deg2RadD(180.0f)) * vel },
		 cameraAngle.y + AsoUtility::Deg2RadF(180.0f));
	move(CheckHitKey(KEY_INPUT_A), pos_, { sinf(cameraAngle.y + AsoUtility::Deg2RadD(-90.0f)) * vel ,0.0f,cosf(cameraAngle.y + AsoUtility::Deg2RadD(-90.0f)) * vel}
	, cameraAngle.y + AsoUtility::Deg2RadF(-90.0f));
	move(CheckHitKey(KEY_INPUT_D), pos_, { sinf(cameraAngle.y + AsoUtility::Deg2RadD(90.0f)) * vel ,0.0f,cosf(cameraAngle.y + AsoUtility::Deg2RadD(90.0f)) * vel }
	, cameraAngle.y + AsoUtility::Deg2RadF(90.0f));
	//move(CheckHitKey(KEY_INPUT_Q), pos_, { 0.0f,vel,0.0f });
	//move(CheckHitKey(KEY_INPUT_E), pos_, { 0.0f,-vel,0.0f });

	camera->SetTardet(pos_);
	// 角度設定
	auto angle = angle_;
	angle.x += localAngle_.x;
	angle.y += localAngle_.y;
	angle.z += localAngle_.z;
	MV1SetRotationXYZ(modelHandle_, angle);
	MV1SetPosition(modelHandle_, pos_);
}

void Unit::Draw()
{
	// モデル描画
	MV1DrawModel(modelHandle_);
}

void Unit::Relese()
{
	MV1DeleteModel(modelHandle_);
}

VECTOR Unit::GetPos()
{
	return pos_;
}

VECTOR Unit::GetAngle()
{
	return angle_;
}
