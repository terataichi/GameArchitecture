#pragma once
#include "Transform.h"

class Rock
{
public:
	Rock(int modelID, VECTOR pos, VECTOR angle , VECTOR Scale);
	~Rock();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	Transform mTransform;
};

