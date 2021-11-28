#include "EventScene.h"
#include "SceneManager.h"
#include "Camera.h"
#include "BossShip.h"
#include "SpaceDome.h"

EventScene::EventScene(SceneManager* manager):
	SceneBase(manager),scene(manager)
{
}

void EventScene::Init(void)
{
	mSceneManager->GetCamera()->ChangeMode(CAMERA_MODE::FIXED);

	boss = new BossShip(scene, nullptr);
	boss->Init();

	doom = new SpaceDome(scene);
	doom->Init();
}

void EventScene::Update(void)
{
	boss->Update();
	doom->Update();
}

void EventScene::Draw(void)
{
	doom->Draw();
	boss->Draw();
}

void EventScene::Release(void)
{
	doom->Release();
	boss->Release();
}
