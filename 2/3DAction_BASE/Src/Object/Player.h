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

	// 状態
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

	// アニメーション種別
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

	// 操作
	void ProscessMove(void);
	void ProscessJump(void);
	// 回転
	void SetGoalRotate(float rad);
	void Rotate(void);

	// 移動量の計算
	void CalcGravityPow();
	// 衝突判定
	void AddCollider(Collider* col);
	void ClearCollider(void);
private:

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	GravityManager* mGravityManager;

	Transform mTransform;

	// アニメーション
	AnimationController* mAnimationController;

	// 状態
	STATE mState;

	// 状態遷移
	void ChangeState(STATE state);

	// 移動スピード
	float mSpeed;
	// 移動方向
	VECTOR mMoveDir;
	// 移動量
	VECTOR mMovePow;
	// 移動後の座標
	VECTOR mMovedPos;

	// 回転
	Quaternion mPlayerRotY;
	Quaternion mGoalQuaRotY;
	float mStepRotTime;

	VECTOR mJumpPow;

	Capsule* mCapsule;

	// ジャンプ中
	bool mIsJump;

	// ジャンプ入力受付
	float mStepJump;

	std::vector<Collider*> mColliders;

	VECTOR mGravHitDown;
	VECTOR mGravHitUp;

	int mImgShadow;

	// 着地モーション
	bool IsEndLanding(void);

	void Collision(void);
	void CollisionGracity(void);
	void CollisionCapsule(void);
};

