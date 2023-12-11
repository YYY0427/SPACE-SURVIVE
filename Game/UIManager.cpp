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
	// ‘¶Ýƒtƒ‰ƒO‚ªfalse‚Ì‚à‚Ì‚ðíœ
	uiStoreTable_.remove_if([](Store ui) { return !ui.ui; });
	uiStoreTable_.remove_if([](Store ui) { return !ui.ui->IsEnabled(); });
}

void UIManager::AddStoreUI(std::shared_ptr<UIBase> ui, Vector2 vec, float speed)
{
	// Šù‚ÉŠi”[‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ÌŠm”F
	for (auto& uiT : uiStoreTable_)
	{
		if (uiT.ui == ui) return;
	}

	// Ši”[
	Store store;
	store.ui = ui;
	store.vec = vec.Normalized() * speed;
	uiStoreTable_.push_back(store);
}

void UIManager::StoreUI()
{
	// Ši”[
	for (auto& uiT : uiStoreTable_)
	{
		uiT.ui->Store(uiT.vec);
	}
}
