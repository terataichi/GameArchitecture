#pragma once

class SceneManager;

class Stage
{
public:
	Stage(SceneManager* scene);
	~Stage();

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	void DrawGrid();

	SceneManager* mSceneManager;
};

