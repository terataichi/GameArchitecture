#include <math.h>
#include "DxLib.h"
#include "GameCommon.h"
#include "SceneManager.h"
#include "Quaternion.h"
#include "AsoUtility.h"
#include "Camera.h"

Camera::Camera(SceneManager* manager)
{
	mSceneManager = manager;
}

Camera::~Camera()
{
}

void Camera::Init()
{
	mPos = { 0.0f,50.0f,-100.0f };
	mTargetPos = { 0.0f,50.0f,100.0f };
	mUp = { 0.0f,1.0f,0.0f };
}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{
	SetCameraPositionAndTargetAndUpVec(mPos, mTargetPos, mUp);
}

void Camera::Draw()
{
}

void Camera::Release(void)
{
}
