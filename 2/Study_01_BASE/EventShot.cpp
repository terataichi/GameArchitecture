#include "EventShot.h"
#include "SpriteAnimator.h"
#include "Camera.h"
#include "SceneManager.h"

EventShot::EventShot(SceneManager* manager, Transform* transform) :
	ShotBase(manager, transform)
{
}

void EventShot::Update(void)
{
	// ���e���̃G�t�F�N�g
	mExplosion->Update();

	// �ړ�����
	Move();

	VECTOR pos2D = ConvWorldPosToScreenPos(mTransform.pos);
	if (pos2D.z <= 0 || pos2D.z >= 1)
	{
		CreateExplosion();
	}
	// ���f������̊�{���X�V
	mTransform.Update();
}

float EventShot::GetSpeed(void)
{
	return 20.0f;
}

float EventShot::GetTimeAlive(void)
{
	return 10.0f;
}

