#pragma once
#include "Vector2.h"
class Camera;

class MiniCamera
{

public:

	MiniCamera(Camera* camera);
	~MiniCamera();

	void Init(void);
	void Update(void);
	void DrawScreen(void);
	void Draw(void);
	void Release(void);

	int GetScreen(void);

private:

	Camera* mCamera;

	int mScreen;

	int mModel;
	
	Vector2 mSize;

};

