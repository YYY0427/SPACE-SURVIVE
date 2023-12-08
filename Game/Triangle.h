#pragma once
#include "Vector2.h"
#include "Util/Timer.h"
#include <array>
#include <vector>
#include <DxLib.h>
#include "UIBase.h"

class Triangle : public UIBase
{
public:
	// コンストラクタ
	Triangle(int num, int frame);

	// デストラクタ
	~Triangle();

	// 更新
	void Update(VECTOR pos);

	// 描画
	void Draw();

	void SetDraw(bool isDraw);

private:
	struct Data
	{
		std::array<Vector2, 3> pos;
		float alpha;
		float angle;
	};

	VECTOR color_;

	int num_;
	
	bool isDraw_;

	// 何フレームで演出を行うか
	Timer<int> frame_;

	// 何フレームの間隔で描画するか
	Timer<int> intarvalFrame;

	std::vector<Data> data_;
};