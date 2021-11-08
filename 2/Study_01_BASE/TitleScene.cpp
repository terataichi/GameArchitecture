#include "DxLib.h"
#include "GameCommon.h"
#include "KeyCheck.h"
#include "SceneManager.h"
#include "Camera.h"
#include "TitleScene.h"
#include "SpaseDome.h"
TitleScene::TitleScene(SceneManager* manager) : SceneBase(manager)
{
}

void TitleScene::Init(void)
{

	mImgStartLogo = LoadGraph("Image/StartLogo.png");
	mSpaseDome = new SpaseDome(mSceneManager, nullptr);
}

void TitleScene::Update(void)
{

	// ÉVÅ[ÉìëJà⁄
	if (keyTrgDown[KEY_SYS_START])
	{
		mSceneManager->ChangeScene(SceneManager::SCENE_ID::GAME, true);
	}
	
	mSpaseDome->Update();
}

void TitleScene::Draw(void)
{
	mSpaseDome->Draw();
	// ÉçÉSï`âÊ
	DrawLogo();
}

void TitleScene::DrawLogo(void)
{

	int cx = SCREEN_SIZE_X / 2;
	int cy = SCREEN_SIZE_Y / 2;
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, mImgStartLogo, true);

	char* msg = (char*)"Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg);
	int width = GetDrawStringWidth(msg, len);
	DrawFormatString((SCREEN_SIZE_X / 2) - (width / 2), 200, 0x87cefa, msg);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
	
}

void TitleScene::Release(void)
{

	DeleteGraph(mImgStartLogo);

}
