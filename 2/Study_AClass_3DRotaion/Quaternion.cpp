#include <math.h>
#include "DxLib.h"
#include "AsoUtility.h"
#include "Quaternion.h"

Quaternion::Quaternion(void)
{
    w = 1;
    x = y = z = 0;
}

Quaternion::Quaternion(VECTOR rad)
{
    Quaternion q = Euler(rad.x, rad.y, rad.z);
    w = q.w;
    x = q.x;
    y = q.y;
    z = q.z;
}

Quaternion::Quaternion(double ww, double wx, double wy, double wz)
{
    w = ww;
    x = wx;
    y = wy;
    z = wz;
}

Quaternion Quaternion::Euler(VECTOR rad)
{
    return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(double radX, double radY, double radZ)
{

    Quaternion ret;

    radX = AsoUtility::RadIn2PI(radX);
    radY = AsoUtility::RadIn2PI(radY);
    radZ = AsoUtility::RadIn2PI(radZ);

    double cosZ = cos(radZ / 2.0f);
    double sinZ = sin(radZ / 2.0f);
    double cosX = cos(radX / 2.0f);
    double sinX = sin(radX / 2.0f);
    double cosY = cos(radY / 2.0f);
    double sinY = sin(radY / 2.0f);

    //ret.w = cosZ * cosX * cosY + sinZ * sinX * sinY;
    //ret.x = sinZ * cosX * cosY - cosZ * sinX * sinY;
    //ret.y = cosZ * sinX * cosY + sinZ * cosX * sinY;
    //ret.z = cosZ * cosX * sinY - sinZ * sinX * cosY;

    ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
    ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
    ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
    ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

    return ret;

}

Quaternion Quaternion::Mult(Quaternion q1, Quaternion q2)
{

    Quaternion ret;
    double d1, d2, d3, d4;

    // w‚ÌŒvŽZ 
    d1 = q1.w * q2.w;
    d2 = -q1.x * q2.x;
    d3 = -q1.y * q2.y;
    d4 = -q1.z * q2.z;
    ret.w = d1 + d2 + d3 + d4;

    // x‚ÌŒvŽZ 
    d1 = q1.w * q2.x;
    d2 = q2.w * q1.x;
    d3 = q1.y * q2.z;
    d4 = -q1.z * q2.y;
    ret.x = d1 + d2 + d3 + d4;

    // y‚ÌŒvŽZ
    d1 = q1.w * q2.y;
    d2 = q2.w * q1.y;
    d3 = q1.z * q2.x;
    d4 = -q1.x * q2.z;
    ret.y = d1 + d2 + d3 + d4;

    // z‚ÌŒvŽZ
    d1 = q1.w * q2.z;
    d2 = q2.w * q1.z;
    d3 = q1.x * q2.y;
    d4 = -q1.y * q2.x;
    ret.z = d1 + d2 + d3 + d4;

    return ret;

}

Quaternion Quaternion::Mult(Quaternion q)
{
    return Mult(Quaternion(w, x, y, z), q);
}

Quaternion Quaternion::AngleAxis(double rad, VECTOR axis)
{

    Quaternion ret;

    double norm;
    double c, s;

    ret.w = ret.x = ret.y = ret.z = 0.0;

    norm = axis.x * axis.x + axis.y * axis.y + axis.z * axis.z;
    if (norm <= 0.0f)
    {
        return ret;
    }

    norm = 1.0f / sqrt(norm);
    axis.x *= norm;
    axis.y *= norm;
    axis.z *= norm;

    c = cos(0.5f * rad);
    s = sin(0.5f * rad);

    ret.w = c;
    ret.x = s * axis.x;
    ret.y = s * axis.y;
    ret.z = s * axis.z;

    return ret;

}

VECTOR Quaternion::ToEuler(Quaternion q)
{

    VECTOR ret;

    double r11 = 2 * (q.x * q.z + q.w * q.y);
    double r12 = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    double r21 = -2 * (q.y * q.z - q.w * q.x);
    double r31 = 2 * (q.x * q.y + q.w * q.z);
    double r32 = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;

    ret.x = asin(r21);
    ret.y = atan2(r11, r12);
    ret.z = atan2(r31, r32);

    return ret;

}

VECTOR Quaternion::ToEuler(void)
{
    return ToEuler(Quaternion(w, x, y, z));
}

MATRIX Quaternion::ToMatrix(Quaternion q)
{

    float sqw = q.w * q.w;
    float sqx = q.x * q.x;
    float sqy = q.y * q.y;
    float sqz = q.z * q.z;
    float invs = 1.0f / (sqx + sqy + sqz + sqw);

    MATRIX matrix = MGetIdent();

    matrix.m[0][0] = (sqx - sqy - sqz + sqw) * invs;
    matrix.m[1][1] = (-sqx + sqy - sqz + sqw) * invs;
    matrix.m[2][2] = (-sqx - sqy + sqz + sqw) * invs;

    float tmp1 = q.x * q.y;
    float tmp2 = q.z * q.w;
    matrix.m[0][1] = 2.0 * (tmp1 + tmp2) * invs;
    matrix.m[1][0] = 2.0 * (tmp1 - tmp2) * invs;

    tmp1 = q.x * q.z;
    tmp2 = q.y * q.w;
    matrix.m[0][2] = 2.0 * (tmp1 - tmp2) * invs;
    matrix.m[2][0] = 2.0 * (tmp1 + tmp2) * invs;

    tmp1 = q.y * q.z;
    tmp2 = q.x * q.w;
    matrix.m[1][2] = 2.0 * (tmp1 + tmp2) * invs;
    matrix.m[2][1] = 2.0 * (tmp1 - tmp2) * invs;

    return matrix;

}

MATRIX Quaternion::ToMatrix(void)
{
    return ToMatrix(Quaternion(w, x, y, z));
}

float Quaternion::Dot(Quaternion q1, Quaternion q2)
{
    return (q1.w * q2.w + q1.x * q2.x * q1.y * q2.y + q1.z * q2.z);
}

float Quaternion::Dot(Quaternion q)
{
    return (w * q.w + x * q.x + y * q.y + z * q.z);
}

Quaternion Quaternion::Normalized(void)
{

    float mag = sqrtf(w * w + x * x + y * y + z * z);
    return Quaternion(w / mag, x / mag, y / mag, z / mag);

}

void Quaternion::Normalize(void)
{

    float mag = sqrtf(w * w + x * x + y * y + z * z);

    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;

}

Quaternion Quaternion::Inverse(void)
{

    double n = 1.0f / (w * w + x * x + y * y + z * z);
    Quaternion tmp = Quaternion(w, -x, -y, -z);
    return Quaternion(tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n);;

}
