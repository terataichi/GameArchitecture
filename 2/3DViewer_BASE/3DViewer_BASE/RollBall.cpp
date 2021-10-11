#include "RollBall.h"
#include "SceneManager.h"
#include "Unit.h"
#include "AsoUtility.h"
#include "_debug/_DebugConOut.h"

RollBall::RollBall(SceneManager* mng, Unit* unit):mSceneMng_(mng),unit_(unit)
{
    Init();
}

RollBall::~RollBall()
{
}

void RollBall::Init()
{
    pos_ = unit_->GetPos();
}

void RollBall::Update()
{
    deltaStep_ += mSceneMng_->GetDeltaTime();

    VECTOR uPos = unit_->GetPos(); 

    pos_.y = 50.0f;

    float radius = 100.0f;
    float speed = 1.0f;
    pos_.x = uPos.x + (radius * sinf(deltaStep_ * speed));
    pos_.z = uPos.z + (radius * cosf(deltaStep_ * speed));

    
    float angle;
    angle = atan2f(pos_.x - uPos.x, pos_.z - uPos.z);
    
    float uAngle = unit_->GetAngle().y;
    angle_ = AsoUtility::RadIn2PI(unit_->GetAngle().y - angle);

}

void RollBall::Draw()
{
    DrawSphere3D(pos_, 10.0f, 5, 0xff0000, 0xff0000, true);
}

void RollBall::Relese()
{
}

VECTOR RollBall::GetPos()
{
    return pos_;
}

float RollBall::GetAngle()
{
    return angle_;
}
