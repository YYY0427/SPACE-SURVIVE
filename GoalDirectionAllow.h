#pragma once
#include <DxLib.h>

// ゴールの方向を示す矢印
class GoalDirectionAllow
{
public:
	// コンストラクタ
	GoalDirectionAllow();

	// デストラクタ
	virtual ~GoalDirectionAllow();

	// 更新
	void Update();
	
	// 描画
	void Draw();

private:
};