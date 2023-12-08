#pragma once
#include "UIBase.h"
#include <vector>
#include <memory>

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

	void Update();
	void Draw();

	std::shared_ptr<UIBase> CreateUI(std::shared_ptr<UIBase> ui);

private:
	std::vector<std::shared_ptr<UIBase>> uiTable_;
};