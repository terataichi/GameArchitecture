#include <string>
#include <Effekseer.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Common/SpeechBalloon.h"
#include "Planet.h"
#include "Player.h"

Player::Player(SceneManager* manager)
{
	mSceneManager = manager;
	mResourceManager = manager->GetResourceManager();
	mGravityManager = manager->GetGravityManager();

	mAnimationController = nullptr;
	mState = STATE::NONE;
}

void Player::Init(void)
{

	// ���f���̊�{�ݒ�
	mTransform.SetModel(mResourceManager->LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	mTransform.scl = AsoUtility::VECTOR_ONE;
	mTransform.pos = { 0.0f, -30.0f, 0.0f };
	mTransform.quaRot = Quaternion();
	mTransform.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	mTransform.Update();

	// �ۉe
	mImgShadow = LoadGraph("Data/Image/Shadow.png");

	// �W�����v���̕ϐ�������
	mIsJump = false;
	mStepJump = 0.0f;
	mJumpPow = AsoUtility::VECTOR_ZERO;

	mCapsule = new Capsule(&mTransform);
	mCapsule->SetRelativePosTop({ 0.0f,110.0f,0.0f });
	mCapsule->SetRelativePosDown({ 0.0f,30.0f,0.0f });
	mCapsule->SetRadius(20.0f);

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	ChangeState(STATE::PLAY);

	mStepRotTime = TIME_ROT;
}

void Player::InitAnimation(void)
{

	std::string path = "Data/Model/Player/";
	mAnimationController = new AnimationController(mSceneManager, mTransform.modelId);
	mAnimationController->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	mAnimationController->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	mAnimationController->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	mAnimationController->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);

	mAnimationController->Play((int)ANIM_TYPE::IDLE);

}

void Player::Update(void)
{

	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		UpdatePlay();
		break;
	case Player::STATE::WARP_RESERVE:
		break;
	case Player::STATE::WARP_MOVE:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

	mTransform.Update();
	mAnimationController->Update();
	
}

void Player::UpdatePlay(void)
{
	// �ړ��ʂ���鏈��
	ProscessMove();
	ProscessJump();
	Rotate();

	// �d�͂ɂ��ړ��ʌv�Z
	CalcGravityPow();

	// �Փ˔���
	Collision();

    mTransform.quaRot = mGravityManager->GetTransform()->quaRot;
	mTransform.quaRot = mTransform.quaRot.Mult(mPlayerRotY);
}

void Player::Draw(void)
{

	// ���f���̕`��
	MV1DrawModel(mTransform.modelId);

	DrawShadow();

	// �f�o�b�O�p�`��
	DrawDebug();

}

void Player::DrawDebug(void)
{

	int white = 0xffffff;
	int black = 0x000000;
	int red = 0xff0000;
	int green = 0x00ff00;
	int blue = 0x0000ff;
	int yellow = 0xffff00;
	int purpl = 0x800080;

	VECTOR v;

	// �L������{���
	//-------------------------------------------------------
	// �L�������W
	v = mTransform.pos;
	DrawFormatString(20, 60, black, "�L�������W �F (%0.2f, %0.2f, %0.2f)",
		v.x, v.y, v.z
	);

	mCapsule->Draw();
	//-------------------------------------------------------

}

void Player::DrawShadow(void)
{
	constexpr float PLAYER_SHADOW_HEIGHT = 300.0f;
	constexpr float PLAYER_SHADOW_SIZE = 30.0f;


	int i, j;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c : mColliders)
	{
		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��

		ModelHandle = c->mModelId;
		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(ModelHandle, 
			-1,
			mTransform.pos,
			VAdd(mTransform.pos, VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > mTransform.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - mTransform.pos.y) / PLAYER_SHADOW_HEIGHT);

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - mTransform.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - mTransform.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, mImgShadow, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);

}

void Player::Release(void)
{
	DeleteGraph(mImgShadow);
}

Transform* Player::GetTransform(void)
{
	return &mTransform;
}

void Player::ProscessMove(void)
{
	// �ړ��ʃ��Z�b�g
	mMovePow = AsoUtility::VECTOR_ZERO;

	// �J�����̉�]�����炤
	Quaternion cameraRot = mSceneManager->GetCamera()->GetQuaRotOutX();

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	double rotRad = 0.0;
	
	if (CheckHitKey(KEY_INPUT_W))
	{
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadD(0.0);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadD(270.0);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadD(180.0);
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadD(90.0);
	}

	// ���͂���������dir�ɉ��������Ă�
	if (!AsoUtility::EqualsVZero(dir) && (mIsJump || IsEndLanding()))
	{
		if (!mIsJump)
		{
			// �����Ă邩�ǂ����ŃA�j���[�V�����ƈړ��X�s�[�h�����߂�
			if (CheckHitKey(KEY_INPUT_RSHIFT))
			{	// �_�b�V��
				mSpeed = SPEED_RUN;
				mAnimationController->Play((int)ANIM_TYPE::FAST_RUN);
			}
			else
			{	// ����
				mSpeed = SPEED_MOVE;
				mAnimationController->Play((int)ANIM_TYPE::RUN);
			}
		}
		else
		{
			if (!mIsJump && IsEndLanding())
			{

			}
		}
		mMovePow = VScale(dir, mSpeed);

		//mStepRotTime = TIME_ROT;
		// �U�������������]�����߂�
		SetGoalRotate(rotRad);

	}
	else
	{
		// �ҋ@���
		mAnimationController->Play((int)ANIM_TYPE::IDLE);
		//mStepRotTime = TIME_ROT;
	}
}

void Player::ProscessJump(void)
{
	if (CheckHitKey(KEY_INPUT_BACKSLASH) || !IsEndLanding())
	{
		if (!mIsJump)
		{
			mAnimationController->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			mAnimationController->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		mStepJump += mSceneManager->GetDeltaTime();
		if (mStepJump < TIME_JUMP_IN)
		{
			mJumpPow = VScale(mGravityManager->GetDirGravity(), POW_JUMP);
			mIsJump = true;
		}
	}

	if (!CheckHitKey(KEY_INPUT_BACKSLASH))
	{
		mStepJump = TIME_JUMP_IN;
	}
}

void Player::SetGoalRotate(float rad)
{
	auto cameraRot = mSceneManager->GetCamera()->GetAngles();
	auto axis = Quaternion::AngleAxis((double)cameraRot.y + rad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ��̊p�x�������߂�
	auto angleDiffDeg = Quaternion::Angle(axis, mGoalQuaRotY);

	// �������l���������
	if (abs(angleDiffDeg) > 30.0)
	{
		mStepRotTime = TIME_ROT;
	}

	mGoalQuaRotY = axis;

}

void Player::Rotate(void)
{
	mStepRotTime -= mSceneManager->GetDeltaTime();
	mPlayerRotY = Quaternion::Slerp(
		mPlayerRotY, mGoalQuaRotY, (TIME_ROT - mStepRotTime) / TIME_ROT);
}

void Player::CalcGravityPow()
{
	// �d�͕���
	VECTOR dirGrav = mGravityManager->GetDirGravity();

	// �d�͂̋���
	float gravityPow = mGravityManager->GetPower();

	VECTOR g = VScale(dirGrav, gravityPow);

	float dot = VDot(dirGrav, mJumpPow);

	if (dot < 0.0f)
	{
		mJumpPow = VAdd(mJumpPow, g);
	}
}

void Player::AddCollider(Collider* col)
{
	mColliders.push_back(col);
}

void Player::ClearCollider(void)
{
	mColliders.clear();
}

void Player::ChangeState(STATE state)
{

	mState = state;
	switch (mState)
	{
	case Player::STATE::NONE:
		break;
	case Player::STATE::PLAY:
		break;
	case Player::STATE::WARP_RESERVE:
		break;
	case Player::STATE::WARP_MOVE:
		break;
	case Player::STATE::DEAD:
		break;
	case Player::STATE::VICTORY:
		break;
	case Player::STATE::END:
		break;
	}

}

bool Player::IsEndLanding(void)
{
	if (mAnimationController->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return true;
	}
	if (mAnimationController->IsEnd())
	{
		return true;
	}
	return false;
}

void Player::Collision(void)
{
	mMovedPos = VAdd(mTransform.pos, mMovePow);

	if (mIsJump)
	{
		mMovedPos = VAdd(mTransform.pos, mJumpPow);
	}
	//�@�d�͔���
	CollisionGracity();

	mTransform.pos = mMovedPos;
}

void Player::CollisionGracity(void)	
{
	auto dir = mGravityManager->GetDirGravity() ;
	auto pow = mGravityManager->GetPower();

	mMovedPos = VAdd(mMovedPos,VScale(dir, pow));

	float checkPow = 10.0f;
	mGravHitUp = VAdd(mMovedPos, VScale(mGravityManager->GetDirUpGravity(), pow));
	mGravHitUp = VAdd(mGravHitUp, VScale(mGravityManager->GetDirUpGravity(), checkPow * 2.0f));
	mGravHitDown = VAdd(mMovedPos, VScale(dir, checkPow));

	for (auto& c : mColliders)
	{
		auto hit = MV1CollCheck_Line(c->mModelId, -1, mGravHitUp, mGravHitDown);

		if (hit.HitFlag > 0)
		{
			// �Փ˒n�_����A������Ɉړ�
			mMovedPos = VAdd(hit.HitPosition, VScale(dir, 2.0f));
			// �W�����v���Z�b�g
			mJumpPow = AsoUtility::VECTOR_ZERO;
			mStepJump = 0.0f;

			if (mIsJump)
			{
				mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			mIsJump = false;
		}
	}
}

void Player::CollisionCapsule(void)
{
}
