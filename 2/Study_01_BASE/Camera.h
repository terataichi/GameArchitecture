#pragma once
#include <vector>
#include "DxLib.h"
#include "Quaternion.h"
class SceneManager;
class Transform;

class Camera
{

public:

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu�ƒ����_�Ƃ̑��΍��W
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };
	
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW = { 0.0f,25.0f,-80.0f };
	static constexpr VECTOR RELATIVE_CAMERA_FOLLOW_SPRIBNG = { 0.0f,20.0f,-70.0f };

	enum class MODE
	{
		NONE,
		FREE,			// ���R���[�h
		FIXSED_POINT,	// ��_���[�h
		FOLLOW,			// �Ǐ]���[�h
		FOLLOW_SPRING	// �΂˕t��
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

	// �����̐ݒ�
	void SetShip(Transform* trans);
private:

	SceneManager* mSceneManager;

	// �J�����̈ʒu
	VECTOR mPos;

	// �J�����p�x
	Quaternion mQuaRot;

	// �����_
	VECTOR mTargetPos;

	// �J�����̏����
	VECTOR mCameraUp;

	MODE mMode;

	Transform* mShipTransform;


	// �΂˗p
	VECTOR mVelosity;

};

