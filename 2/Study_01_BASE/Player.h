#pragma once
#include "Transform.h"

class SceneManager;
class ParticleGenerator;

class Player
{
public:

	Player(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform GetTransForm(void);

	void ProcessTurn(void);
	void Turn(float angle, VECTOR axis);
private:
	SceneManager* mSceneManager;
	ParticleGenerator* mParticleGenerator;
	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform mTransform;
};

