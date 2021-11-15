#pragma once
#include "Transform.h"

class SceneManager;
class ParticleGenerator;
class SpriteAnimator;
class SpeechBalloon;
class Player
{
public:
	enum class STATE
	{
		Non,
		Run,
		Destroy
	};
	static constexpr float COLLISION_RADIUS = 20.0f;

	Player(SceneManager* manager);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform GetTransForm(void);

	void ProcessTurn(void);
	void Turn(float angle, VECTOR axis);

	SpeechBalloon* GetSpeech();

	void SetState(STATE state);
	STATE GetState();
private:
	void RunUpdate();
	void DestroyUpdate();

	SpeechBalloon* mSpeechBalloon;
	SceneManager* mSceneManager;
	ParticleGenerator* mParticleGenerator;
	SpriteAnimator* mExprosion;
	// ÉÇÉfÉãêßå‰ÇÃäÓñ{èÓïÒ
	Transform mTransform;
	STATE mState;
};

