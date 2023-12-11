#pragma once
#include "UIBase.h"
#include <list>
#include <memory>
#include "Vector2.h"

enum class ID
{
	HP,
	WARNING,
	FLASH,
	TRIANGLE
};

class UIManager
{
public:
	UIManager();
	~UIManager();

	void DeleteCheck();

	void AddStoreUI(std::shared_ptr<UIBase> ui, Vector2 vec, float speed);

	void StoreUI();

private:
	struct Store
	{
		std::shared_ptr<UIBase> ui;
		Vector2 vec;
	};
	
private:
	// �Q�[���N���A����Q�[���I�[�o�[���Ɋi�[����UI
	std::list<Store> uiStoreTable_;
};