#pragma once
#include <DxLib.h>
#include "Common/Transform.h"
class SceneManager;
class ResourceManager;
class GravityManager;
class AnimationController;
class Capsule;
class Collider;

class Player
{

public:
	// speed
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;
	static constexpr float TIME_ROT = 1.0f;
	static constexpr float POW_JUMP = 10.0f;
	static constexpr float TIME_JUMP_IN = 0.5f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	Player(SceneManager* manager);
	void Init(void);
	void InitAnimation(void);
	void Update(void);
	void UpdatePlay(void);
	void Draw(void);
	void DrawDebug(void);
	void DrawShadow(void);
	void Release(void);

	Transform* GetTransform(void);

	// ����
	void ProscessMove(void);
	void ProscessJump(void);
	// ��]
	void SetGoalRotate(float rad);
	void Rotate(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow();
	// �Փ˔���
	void AddCollider(Collider* col);
	void ClearCollider(void);
private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// �A�j���[�V����
	AnimationController* mAnimationController;

	// ���
	STATE mState;

	// ��ԑJ��
	void ChangeState(STATE state);

	// �ړ��X�s�[�h
	float mSpeed;
	// �ړ�����
	VECTOR mMoveDir;
	// �ړ���
	VECTOR mMovePow;
	// �ړ���̍��W
	VECTOR mMovedPos;

	// ��]
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	VECTOR mJumpPow;

	Capsule* mCapsule;

	// �W�����v��
	bool mIsJump;

	// �W�����v���͎�t
	float mStepJump;

	std::vector<Collider*> mColliders;

	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	int mImgShadow;

	// ���n���[�V����
	bool IsEndLanding(void);

	void Collision(void);
	void CollisionGracity(void);
	void CollisionCapsule(void);
};

