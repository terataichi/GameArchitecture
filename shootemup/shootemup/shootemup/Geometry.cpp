#include"Geometry.h"
#include<DxLib.h>
#include<cmath>
#include <algorithm>

const Vector2 Vector2::ZERO = { 0.0f,0.0f };

void
Rect::Draw() {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void
Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
}

void
Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2
Vector2::operator*(float scale) {
	return Vector2(x*scale, y*scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb){
	return Vector2(va.x - vb.x, va.y - vb.y);
}

Vector2 operator/(const Vector2& va, const Vector2 vb)
{
	return Vector2(va.x / vb.x, va.y / vb.y);
}

bool operator==(const Vector2& va, const Vector2 vb)
{
	return va.x == vb.x && va.y == vb.y;
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}

float Vector2::SQMagnitude() const
{
	return x * x + y * y;
}


void 
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized() {
	Vector2 mag = { x,y };
	mag.Normalize();
	return mag;
}

void Vector2::Rotate90()
{
	//float tmpX = x;
	//x = -y;
	//y = tmpX;
	std::swap(x, y);
	x = -x;
}

Vector2 Vector2::Rotated90()
{
	return { -y,x };
}


///���ς�Ԃ�
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///�O�ς�Ԃ�
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///���ω��Z�q
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
float 
operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}


void 
Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}
void 
Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}


//-------��������3D�̃^�[��------
void
Vector3::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3
Vector3::operator*(float scale)const {
	return Vector3(x*scale, y*scale,z*scale);
}

Vector3 
Vector3::operator*(const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 operator+(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x + vb.x, va.y + vb.y,va.z+vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x - vb.x, va.y - vb.y,va.z-vb.z);
}

float
Vector3::Magnitude()const {
	return sqrt(x * x + y * y + z * z);
}


void
Vector3::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}


Vector3
Vector3::Normalized() {
	float mag = Magnitude();
	return Vector3(x / mag, y / mag,z/mag);
}


///���ς�Ԃ�
// 0�𒆐S�ɐ��Ώ�
float
Dot(const Vector3& va, const Vector3& vb) {
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}

///�O�ς�Ԃ�
// 0�𒆐S�ɓ_�Ώ�
Vector3
Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}

///���ω��Z�q
float
operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
Vector3
operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}


void
Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
void
Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

Plane::Plane(const Vector3& n, float offset) :N(n), b(offset)
{
	N.Normalize();		// ���K��
}
