#pragma once
#include "Transform.h"

class SceneManager;

class SpaseDome
{
public:
	enum class STATE
	{
		STAY,
		FOLLOW
	};
	SpaseDome(SceneManager* scene,Transform* ship);
	~SpaseDome();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void ChangeState(STATE state);
private:
	STATE mState;

	// モデル情報の基本情報
	Transform mTransform;
	Transform* mShipTransform;
	SceneManager* mSceneManager;
};

