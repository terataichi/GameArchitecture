#include "Coin.h"
#include "AsoUtility.h"
#include "Quaternion.h"

Coin::Coin(SceneManager* manager)
{
	Init();
}

void Coin::Init(void)
{
	mModelId = MV1LoadModel("Model/Coin.mv1");
	mPos = { 0.0f,50.0f,0.0f };
	mAngles = {};

	mType = TYPE::QUATERNION;

	float localX = AsoUtility::Deg2RadF(-90);
	float localY = AsoUtility::Deg2RadF(0);
	float localZ = AsoUtility::Deg2RadF(0);

	mAnglesLocal = { localX,localY,localZ };
	VECTOR angle = mAngles;
	angle = VAdd(angle, mAnglesLocal);


	mMatRotLocal = MGetIdent();
	mMatRotLocal = MMult(mMatRotLocal, MGetRotX(localX));
	mMatRotLocal = MMult(mMatRotLocal, MGetRotY(localY));
	mMatRotLocal = MMult(mMatRotLocal, MGetRotZ(localZ));

	// �N�I�[�^�j�I���̏ꍇ
	mQuaRotLocal = Quaternion::AngleAxis(localX, { 1.0f,0.0f,0.0f });
	mQuaRotLocal = Quaternion::AngleAxis(localY, { 0.0f,1.0f,0.0f });
	mQuaRotLocal = Quaternion::AngleAxis(localZ, { 0.0f,0.0f,1.0f });

	MV1SetPosition(mModelId, mPos);
	MV1SetScale(mModelId, SCALE);
}

void Coin::Update(void)
{
	//mAngles.x += AsoUtility::Deg2RadF(0);
	//mAngles.y += AsoUtility::Deg2RadF(1);
	//mAngles.z += AsoUtility::Deg2RadF(1);
	
	// �W���o�����b�N
	mAngles.x += AsoUtility::Deg2RadF(1);
	mAngles.y = AsoUtility::Deg2RadF(0);
	mAngles.z += AsoUtility::Deg2RadF(0);
	switch (mType)
	{
	case Coin::TYPE::VECTOR:
		SetModelVector();
		break;
	case Coin::TYPE::MATRIX:
		SetModelMatrix();
		break;
	case Coin::TYPE::QUATERNION:
		SetModelQuaternion();
	default:
		break;
	}
}

void Coin::Draw(void)
{
	MV1DrawModel(mModelId);
	DrawDirection();
}

void Coin::Release(void)
{
}

void Coin::SetModelVector()
{
	// �܂��͑傫��
	MV1SetScale(mModelId, SCALE);
	// ���ɉ�]
	VECTOR angle = mAngles;
	angle = VAdd(angle, mAnglesLocal);
	MV1SetRotationXYZ(mModelId, angle);
	// �Ō�ɍ��W
	MV1SetPosition(mModelId, mPos);
}

void Coin::SetModelMatrix()
{
	// �傫��
	mMatScl = MGetScale(SCALE);

	// ��]
	mMatRot = MGetIdent();

	// ���[�J������
	mMatRot = MMult(mMatRot, mMatRotLocal);

	// ��]�̍���
	mMatRot = MMult(mMatRot, MGetRotX(mAngles.x));
	mMatRot = MMult(mMatRot, MGetRotY(mAngles.y));
	mMatRot = MMult(mMatRot, MGetRotZ(mAngles.z));

	// �ʒu
	mMatTrn = MGetTranslate(mPos);
	// �傫��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, mMatScl);
	mat = MMult(mat, mMatRot);
	mat = MMult(mat, mMatTrn);

	// ���[�J����]
	MATRIX rot = MGetIdent();
	rot = MMult(rot, mMatRotLocal);
	rot = MMult(mat, rot);

	MV1SetMatrix(mModelId, mat);


}

void Coin::SetModelQuaternion()
{
	// �傫��
	mMatScl = MGetScale(SCALE);

	// ��]
	Quaternion tmpQ = Quaternion::Euler(mAngles.x, mAngles.y, mAngles.z);

	mMatRot = tmpQ.ToMatrix();

	// �傫��
	MATRIX mat = MGetIdent();
	mat = MMult(mat, mMatScl);
	// �ʒu
	mMatTrn = MGetTranslate(mPos);

	tmpQ = tmpQ.Mult(mQuaRotLocal);
	// ���[�J������
	mat = MMult(mat, tmpQ.ToMatrix());

	mat = MMult(mat, mMatTrn);

	MV1SetMatrix(mModelId, mat);
}

void Coin::DrawDirection()
{
	// �R�C���̌����ɍ��킹�āAXYZ���`��
	MATRIX mat = MGetIdent();

	switch (mType)
	{
	case Coin::TYPE::VECTOR:
		break;
	case Coin::TYPE::MATRIX:
		mat = mMatRot;
		break;
	case Coin::TYPE::QUATERNION:
		mat = mMatRot;
	default:
		break;
	}

	VECTOR foward = VNorm(VTransform({ 0.0f,0.0f,1.0f }, mat));


	VECTOR dir;
	VECTOR s;
	VECTOR e;
	float len = 30.0f;
	float size = 3.0f;

	// Z��
	dir = foward;
	s = VAdd(mPos, VScale(dir, -len));
	e = VAdd(mPos, VScale(dir, len));

	DrawLine3D(s, e, 0x0000ff);
	DrawSphere3D(e, size, 8, 0x0000ff, 0x0000ff,true);

	// X��
	foward = VNorm(VTransform({ 1.0f,0.0f,0.0f }, mat));
	dir = foward;
	s = VAdd(mPos, VScale(dir, -len));
	e = VAdd(mPos, VScale(dir, len));

	DrawLine3D(s, e, 0x00ff00);
	DrawSphere3D(e, size, 8, 0x00ff00, 0x00ff00, true);

	// Y��
	foward = VNorm(VTransform({ 0.0f,1.0f,0.0f }, mat));
	dir = foward;
	s = VAdd(mPos, VScale(dir, -len));
	e = VAdd(mPos, VScale(dir, len));

	DrawLine3D(s, e, 0xff0000);
	DrawSphere3D(e, size, 8, 0xff0000, 0xff0000, true);

}
