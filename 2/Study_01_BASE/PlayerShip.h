#pragma once
#include "Transform.h"

class SceneManager;

class PlayerShip
{
public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// ���@�̊J�X��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	PlayerShip(SceneManager* scene);
	~PlayerShip();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	Transform* GetTransform(void);

	// ���@�̑���
	void ProcessTurn();
	void Turn(bool checkKey, double deg, VECTOR axis);
private:

	// ���f�����̊�{���
	Transform mTransform;

	SceneManager* mSceneManager;
};

