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

void UIManager::DeleteCheck()
{
	// 存在フラグがfalseのものを削除
	uiStoreTable_.remove_if([](Store ui) { return !ui.ui; });
	uiStoreTable_.remove_if([](Store ui) { return !ui.ui->IsEnabled(); });
}

void UIManager::AddStoreUI(std::shared_ptr<UIBase> ui, Vector2 vec, float speed)
{
	// 既に格納されているかどうかの確認
	for (auto& uiT : uiStoreTable_)
	{
		if (uiT.ui == ui) return;
	}

	// 格納
	Store store;
	store.ui = ui;
	store.vec = vec.Normalized() * speed;
	uiStoreTable_.push_back(store);
}

void UIManager::StoreUI()
{
	// 格納
	for (auto& uiT : uiStoreTable_)
	{
		uiT.ui->Store(uiT.vec);
	}
}
