#include "Particle.h"
#include "SceneManager.h"

Particle::Particle(SceneManager* scenemanager, int imageHandleID) :
	sceneManager_(scenemanager), imageHandle_(imageHandleID)
{
	
}

Particle::~Particle() = default;

void Particle::Generate(VECTOR pos, float size, VECTOR dir, float speed, float limitTime)
{
	pos_ = pos;
	size_ = size;
	dir_ = dir;
	speed_ = speed;
	limitTime_ = limitTime;
}

void Particle::Update(void)
{
	float delta = sceneManager_->GetDeltaTime();
	pos_ = VAdd(pos_, VScale(dir_, speed_ * delta));
	limitTime_ -= delta;
}

void Particle::Draw(void)
{
	DrawBillboard3D(pos_, 0.5f, 0.5f, size_, 0.0f, imageHandle_, true);
}

void Particle::Release(void)
{
}
