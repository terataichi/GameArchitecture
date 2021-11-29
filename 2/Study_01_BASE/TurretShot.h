#pragma once
#include "ShotBase.h"

class TurretShot :
    public ShotBase
{
public:
    TurretShot(SceneManager* manager, Transform* dir);

    // 砲身から弾の発射
    // 弾の方向
    // 弾の方向 = 砲身の前方方向
    // 弾の発射位置 = 砲身の位置からの相対位置(Q PosAxsis)
    //                                         ちょっと上（2）、ちょっと前（3）※回転を考慮



    // 弾の発射角度


};

