#pragma once
#include <vector>
#include "DxLib.h"
class Coin;
class SceneManager;

class Camera
{

public:

	// グリッド線の大きさ
	static constexpr float LEN_MASS = 10.0f;

	// グリッド線の描画数
	static constexpr float LEN_NUM = 20.0f;

	Camera(SceneManager* manager);
	~Camera();

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);
	void Release(void);

private:

	SceneManager* mSceneManager;
	Coin* mShip;

	// カメラの位置
	VECTOR mPos;

	// 注視点
	VECTOR mTargetPos;

	// カメラの上方向
	VECTOR mUp;

};

