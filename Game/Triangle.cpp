#include "Triangle.h"
#include "common.h"

Triangle::Triangle(int num, int frame) 
{
	num_ = num;
	frame_ = frame;
	intarvalFrame = frame / (num + 1);
	color_ = { 255, 255, 255 };
}

Triangle::~Triangle()
{
}

void Triangle::Update(VECTOR pos)
{
	frame_.Update(1);
	intarvalFrame.Update(1);
	if (intarvalFrame.IsTimeOut() && data_.size() < num_)
	{
		// タイマーの初期化
		intarvalFrame.Reset();

		Data data;

		VECTOR screenPos = ConvWorldPosToScreenPos({ pos.x, pos.y, pos.z });
		data.pos[0] = Vector2(screenPos.x, screenPos.y);

		// 角度をランダムで取得
		data.angle = GetRand(DX_PI_F * 2);
		for (auto& triangle : data_)
		{
			// 角度が重複していたら再度乱数を取得
			while (data.angle == triangle.angle)
			{
				data.angle = GetRand(DX_PI_F * 2);
			}
		}
		
		// 三角形の頂点を設定
		data.pos[1] = Vector2(cosf(data.angle) * common::screen_width, sinf(data.angle) * common::screen_height);

		// 三角形の頂点を設定
		data.pos[2] = Vector2(data.pos[1].x, data.pos[1].y + 300.0f);

		// アルファ値を設定
		data.alpha = 255;

		// データを追加
		data_.push_back(data);
	}

	for (auto& triangle : data_)
	{
		triangle.alpha *= -1;
		VECTOR screenPos = ConvWorldPosToScreenPos({ pos.x, pos.y, pos.z });
		triangle.pos[0] = Vector2(screenPos.x, screenPos.y);
	}

	if (frame_.GetTime() > frame_.GetLimitTime() * 0.5f)
	{
		color_.y -= 2;
		color_.z -= 2;

		color_.y = (std::max)(color_.y, 0.0f);
		color_.z = (std::max)(color_.z, 0.0f);
	}
}

// 描画
void Triangle::Draw()
{
	if (frame_.IsTimeOut()) return;

	for (auto& triangle : data_)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, triangle.alpha);
		DrawTriangle(triangle.pos[0].x, triangle.pos[0].y, triangle.pos[1].x, triangle.pos[1].y, triangle.pos[2].x, triangle.pos[2].y, GetColor(color_.x, color_.y, color_.z), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
