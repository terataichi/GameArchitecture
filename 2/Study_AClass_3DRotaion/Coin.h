#pragma once
#include "DxLib.h"
#include "Quaternion.h"
class SceneManager;

class Coin
{

public:

	static constexpr float SIZE = 15.0f;
	static constexpr VECTOR SCALE = { SIZE,SIZE,SIZE };

	enum class TYPE
	{
		VECTOR,
		MATRIX,
		QUATERNION
	};


	Coin(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetModelVector();
	void SetModelMatrix();
	void SetModelQuaternion();
private:
	void DrawDirection();

	TYPE mType;


	// ÉÇÉfÉãID
	int mModelId;

	// ç¿ïW
	VECTOR mPos;

	VECTOR mAngles;
	VECTOR mAnglesLocal;

	// çsóÒ
	MATRIX mMatScl;
	MATRIX mMatRot;
	MATRIX mMatTrn;
	MATRIX mMatRotLocal;
	Quaternion mQuaRotLocal;
};

