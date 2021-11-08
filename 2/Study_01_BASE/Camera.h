#pragma once
#include <vector>
#include "DxLib.h"
#include "Quaternion.h"
class SceneManager;
class Transform;

class Camera
{

public:

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置と注視点との相対座標
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };
	
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW = { 0.0f,25.0f,-80.0f };
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW_SPRIBNG = { 0.0f,20.0f,-70.0f };

	enum class MODE
	{
		NONE,
		FREE,			// 自由モード
		FIXSED_POINT,	// 定点モード
		FOLLOW,			// 追従モード
		FOLLOW_SPRING	// ばね付き
	};
	
	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void SetDefault(void);

	void Update(void);

	void SetBeforeDraw(void);

	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixsedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);

	void Draw(void);
	void DrawDebug(void);
	void DrawUI(void);
	void Release(void);

	VECTOR GetPos(void);
	VECTOR GetTargetPos(void);
	VECTOR GetDir(void);
	
	void ChangeMode(MODE mode);

	// 時期の設定
	void SetShip(Transform* trans);
private:

	SceneManager* mSceneManager;

	// カメラの位置
	VECTOR mPos;

	// カメラ角度
	Quaternion mQuaRot;

	// 注視点
	VECTOR mTargetPos;

	// カメラの上方向
	VECTOR mCameraUp;

	MODE mMode;

	Transform* mShipTransform;


	// ばね用
	VECTOR mVelosity;

};

