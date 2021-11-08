#pragma once
#include "DxLib.h"

class Quaternion
{

public:

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);
	Quaternion(VECTOR rad);
	Quaternion(double w, double x, double y, double z);

	// オイラー角からQへ変換
	static Quaternion Euler(VECTOR rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// Qの合成
	static Quaternion Mult(Quaternion q1, Quaternion q2);
	Quaternion Mult(Quaternion q);

	// 指定軸を指定角分、回転させる
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// Qからオイラー角へ変換
	static VECTOR ToEuler(Quaternion q);
	VECTOR ToEuler(void);

	// Qから行列へ変換
	static MATRIX ToMatrix(Quaternion q);
	MATRIX ToMatrix(void);
	
	static float Dot(Quaternion q1, Quaternion q2);
	float Dot(Quaternion b);

	// 正規化
	Quaternion Normalized(void);
	void Normalize(void);

	// 逆クォータニオン
	Quaternion Inverse(void);

};

