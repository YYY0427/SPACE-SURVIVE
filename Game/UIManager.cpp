#include "UIManager.h"
#include "Flash.h"
#include "Triangle.h"
#include "UI/HpBar.h"
#include "UI/Warning.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

std::shared_ptr<UIBase> UIManager::CreateUI(std::shared_ptr<UIBase> ui)
{
	uiTable_.push_back(ui);
	return uiTable_.back();
}
