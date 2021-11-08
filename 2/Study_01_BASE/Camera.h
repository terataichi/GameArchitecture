#pragma once
#include <vector>
#include "DxLib.h"
#include "Quaternion.h"

enum class CAMERA_MODE
{
	NONE,
	FREE,
	FIXED,
	FOLLOW,
	FOLLOW_SPRING
};

class SceneManager;
class Player;

class Camera
{
public:
	static constexpr float MOVE_SPEED = 500.0f;
	static constexpr float ROTATE_SPEED = 90.0f;
	static constexpr float POW = 50.0f;
	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };


	// カメラ位置と注視点との相対座標
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };

	// Playerとカメラとの相対座標
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW = { 0.0f, 25.0f, -80.0f };
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW_SPRING = { 0.0f, 25.0f, 80.0f };
	
	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void SetDefault(void);

	void Update(void);

	void SetBeforeDraw(void);
	void Draw(void);
	void DrawDebug(void);
	void DrawUI(void);
	void Release(void);

	VECTOR GetPos(void);
	VECTOR GetTargetPos(void);
	VECTOR GetDir(void);

	void ChangeMode(CAMERA_MODE mode);

	void SetPlayer(Player* player);

private:

	SceneManager* mSceneManager;
	Player* player_;

	CAMERA_MODE mMode;
	// カメラの位置
	VECTOR mPos;
	// 注視点
	VECTOR mTargetPos;

	// カメラの上方向
	VECTOR mCameraUp;

	VECTOR mVelocity;
	// カメラ角度
	Quaternion mQuaRot;

	void Move();
	void Rotate();

	void SetBeforeDrawFree();
	void SetBeforeDrawFixed();
	void SetBeforeDrawFollow();
	void SetBeforeDrawSpring();
};

