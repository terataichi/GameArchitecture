#include "TextScroll.h"
#include "SceneManager.h"
TextScroll::TextScroll(SceneManager* mng)
{
    mSceneManager = mng;
    Init();
}

void TextScroll::Init()
{
    LoadDivGraph("Image/Alphabet.png",
        NUM_ALL, NUM_X, NUM_Y, SIZE, SIZE, &mImages[0], true);

    mType = Type::Title;

    std::vector<MsgInfo> infos;
    infos.push_back(MakeMsgInfo("Hello", infos.size()));

    infos.push_back(MakeMsgInfo("Bye", infos.size()));

    mTextMap.emplace(mType, infos);

    auto& tmpinfos = mTextMap[mType];
    int i = 0;
    for (auto& info : tmpinfos)
    {
        info.pos_.y = -100 + SIZE * i;
        info.pos_.x = -100;
        i++;
    }
}

void TextScroll::Update(void)
{
    auto& infos = mTextMap[mType];
    for (auto& info : infos)
    {
        info.pos_.y += 0.2f;
    }

}

void TextScroll::Draw(void)
{
    VECTOR pos{};
    auto& infos = mTextMap[mType];
    for (auto& info : infos)
    {
        pos = info.pos_;
        for (auto& msg : info.massage)
        {
            pos.x += SIZE;
            DrawBillboard3D(
                pos,
                0.5, 
                0.5,
                20.0f,
                0.0f,
                mImages[msg],
                true
                );
        }
    }
}

void TextScroll::Release(void)
{
}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(std::string msg, int mapCount)
{

    MsgInfo ret;

    std::vector<int> massages;

    int ascii;
    int len = msg.size();
    for (int i = 0; i < len; i++)
    {
        ascii = msg.at(i);

        if (ascii >= 65 && ascii <= 90)
        {
            ascii -= 65;
        }
        if (ascii >= 97 && ascii <= 122)
        {
            ascii -= 71;
        }
        // スペース変換
        if (ascii == 32)
        {
            ascii = 52;
        }
        if (ascii == 46)
        {
            ascii = 53;
        }      
        if (ascii == 44)
        {
            ascii = 54;
        }

        massages.push_back(ascii);
    }

    ret.pos_ = {0,0,0};
    ret.massage = massages;

    return ret;
}
