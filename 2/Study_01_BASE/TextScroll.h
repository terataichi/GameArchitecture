#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include <string>
class SceneManager;

class TextScroll
{
public:

	static constexpr int NUM_X = 10;
	static constexpr int NUM_Y = 6;
	static constexpr int NUM_ALL = NUM_X * NUM_Y;
	static constexpr int SIZE = 32;

	enum class Type
	{
		Title,
	};

	struct MsgInfo
	{
		VECTOR pos_;
		std::vector<int> massage;
	};

	TextScroll(SceneManager* mng);
	void Init();
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	SceneManager* mSceneManager;
	int mImages[NUM_ALL];
	Type mType;
	std::map<Type, std::vector<MsgInfo>>mTextMap;

	MsgInfo MakeMsgInfo(std::string msg, int mapCount);
};

