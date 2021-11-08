#include "DxLib.h"
#include "GameCommon.h"
#include "Camera.h"
#include "AsoUtility.h"
#include "Transform.h"
#include "SceneManager.h"

Camera::Camera(SceneManager* manager)
{
	mSceneManager = manager;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	ChangeMode(MODE::FREE);
	SetDefault();

	
}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	mPos = DEFAULT_CAMERA_POS;
	mTargetPos = VAdd(mPos, RELATIVE_TARGET_POS);
	mCameraUp = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A���̌X������Ԃ��X�������Ƃ���
	// mQuaRot�͉�]�v�Z�p�ŁA
	// �����܂Ŏ��ƂȂ�̂́A�J�������W�ƒ����_�Ƃ���
	mQuaRot = Quaternion();

}

void Camera::Update()
{



}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(30.0f, 15000.0f);


	switch (mMode)
	{
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FIXSED_POINT:
		SetBeforeDrawFixsedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSpring();
		break;
	default:
		break;
	}

	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		mPos,
		mTargetPos,
		mCameraUp
	);

}

void Camera::SetBeforeDrawFree(void)
{
	// �J�����̕����ړ��Ɖ�](��]�ړ�)

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	auto move = [&](bool check ,VECTOR dir)
	{
		if (check)
		{
			moveDir = dir;
		}
	};
	move(CheckHitKey(KEY_INPUT_W), AsoUtility::DIR_F);
	move(CheckHitKey(KEY_INPUT_A), AsoUtility::DIR_L);
	move(CheckHitKey(KEY_INPUT_S), AsoUtility::DIR_B);
	move(CheckHitKey(KEY_INPUT_D), AsoUtility::DIR_R);


	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	auto  rotate = [&](bool check, VECTOR dir)
	{
		if (check)
		{
			axisDeg = VAdd(axisDeg,dir);
		}
	};
	rotate(CheckHitKey(KEY_INPUT_UP), { -1.0f,0.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_DOWN), { 1.0f,0.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_LEFT), { 0.0f,-1.0f,0.0f });
	rotate(CheckHitKey(KEY_INPUT_RIGHT), { 0.0f,1.0f,0.0f });

	// ��]
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		// ���W�A���ϊ�
		VECTOR axisRad;
		axisRad.x = AsoUtility::Deg2RadF(axisDeg.x);
		axisRad.y = AsoUtility::Deg2RadF(axisDeg.y);
		axisRad.z = AsoUtility::Deg2RadF(axisDeg.z);

		// �N�I�[�^�j�I���X�V
		Quaternion tmpAxis = Quaternion::Euler(axisRad);
		mQuaRot = mQuaRot.Mult(tmpAxis);

		// �����_�X�V
		VECTOR tmpPos = mQuaRot.PosAxis(RELATIVE_TARGET_POS);
		mTargetPos = VAdd(mPos, tmpPos);

		// �J�����̏����
		mCameraUp = mQuaRot.GetUp();
	}

	// �ړ�
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 30.0f;


		MATRIX mat = mQuaRot.ToMatrix();
		// �ړ�������������(�x�N�g��)�����߂���
		VECTOR dir = VNorm(VTransform(moveDir, mat));

		VECTOR movePower = VScale(dir, speed);

		mPos = VAdd(mPos, movePower);
		mTargetPos = VAdd(mTargetPos, movePower);
	}
}

void Camera::SetBeforeDrawFixsedPoint(void)
{
}

void Camera::SetBeforeDrawFollow(void)
{
	if (mShipTransform == nullptr)
	{
		return;
	}

	// �v���C���[���W���擾
	VECTOR shiPos = mShipTransform->pos;
	// �v���C���[�̌����擾
	Quaternion shipRot = mShipTransform->quaRot;

	// �J�����ʒu
	// -------------------------------------------------------------------
	// mPos = VAdd(shiPos, RELATIVE_CAMERA_FOLLOW);
	// -> �v���C���[�����ʂ������Ă��鎞�����Ή��ł��Ȃ�
	//	  ���΍��W�Ɍ������ăv���C���[�̊J�X��������K�v������
	VECTOR soutaiCameraPos;
	soutaiCameraPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW);

	mPos = VAdd(shiPos, soutaiCameraPos);
	// -------------------------------------------------------------------


	// �����_
	// -------------------------------------------------------------------
	// mTargetPos
	VECTOR soutaiTargetPos;
	soutaiTargetPos = shipRot.PosAxis(RELATIVE_TARGET_POS);
	mTargetPos = VAdd(mPos, soutaiTargetPos);
	// -------------------------------------------------------------------

	// �J�����̏����
	// -------------------------------------------------------------------
	// mCameraUp
	mCameraUp = shipRot.GetUp();
	// -------------------------------------------------------------------

	// ���@�ƃJ�����ʒu�Ƃ̑��΍��W
	RELATIVE_CAMERA_FOLLOW;

	// �J�����ʒu��RELATIVE_TARGET_POS���΍��W
	RELATIVE_TARGET_POS;

}

void Camera::SetBeforeDrawFollowSpring(void)
{
	if (mShipTransform == nullptr)
	{
		return;
	}

	// (�萔�\��)
	float POW_SPRING = 24.0f;
	float dampening = 2.0 * sqrt(POW_SPRING);

	float delta = mSceneManager->GetDeltaTime();

	// �v���C���[���W���擾
	VECTOR shiPos = mShipTransform->pos;
	// �v���C���[�̌����擾
	Quaternion shipRot = mShipTransform->quaRot;

	// �J�����̑��Έʒu
	VECTOR soutaiCameraPos;
	soutaiCameraPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW_SPRIBNG);

	// ���z�ʒu
	VECTOR idealPos = VAdd(shiPos, soutaiCameraPos);

	// ���݈ʒu�Ɨ��z�ʒu�̍�
	VECTOR diff = VSub(mPos, idealPos);

	// �΂˂̗�
	VECTOR force = VSub(VScale(diff, -POW_SPRING), VScale(mVelosity, dampening));

	// �J�������x���X�V
	mVelosity = VAdd(mVelosity,VScale(force, delta));
	mPos = VAdd(mPos, VScale(mVelosity, delta));

	// mTargetPos
	VECTOR soutaiTargetPos;
	soutaiTargetPos = shipRot.PosAxis(RELATIVE_CAMERA_FOLLOW_SPRIBNG);
	mTargetPos = VAdd(mPos, soutaiTargetPos);

	mCameraUp = shipRot.GetUp();
}

void Camera::Draw()
{
}

void Camera::DrawDebug(void)
{
}

void Camera::DrawUI(void)
{

	// �������
	int x = 200;
	int y = 10;
	int addY = 40;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�y����z", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�@�@�@����F��������", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�@�@�@�����F�a", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�@�@�@���e�F�m", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�V�[���J�ځF�r��������", 0xffffff);
	y += addY;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@1500m�ȏ㗣����", 0xffffff);
	y += 20;
	DrawString(
		SCREEN_SIZE_X - x, y, "�@�Q�[���I�[�o�[", 0xffffff);

}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void)
{
	return mPos;
}

VECTOR Camera::GetTargetPos(void)
{
	return mTargetPos;
}

VECTOR Camera::GetDir(void)
{
	return VNorm(VSub(mTargetPos, mPos));
}

void Camera::ChangeMode(MODE mode)
{
	SetDefault();
	mMode = mode;
}

void Camera::SetShip(Transform* trans)
{
	mShipTransform = trans;
}
