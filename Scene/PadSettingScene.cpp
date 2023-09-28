#include "PadSettingScene.h"
#include "SceneManager.h"
#include "../Util/StringManager.h"
#include "../Util/InputState.h"
#include "../common.h"

PadSettingScene::PadSettingScene(SceneManager& manager) : 
	Scene(manager)
{
}

PadSettingScene::~PadSettingScene()
{
}

void PadSettingScene::Update()
{
	if (InputState::IsTriggered(InputType::BACK))
	{
		manager_.PopScene();
		return;
	}
}

void PadSettingScene::Draw()
{
	auto& stringManager = StringManager::GetInstance();

	stringManager.DrawStringCenter("PadSettingTitle", common::screen_width / 2, 100, 0xffffff);

}
