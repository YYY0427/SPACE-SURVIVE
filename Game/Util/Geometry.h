#pragma once

// ベクトル構造体
// プレイヤーの座標や速度を表すためのもの
struct Vector2
{
	float x;
	float y;

	Vector2() :
		x(0),
		y(0) {}

	Vector2(float inx, float iny) :
		x(inx),
		y(iny) {}

	// 加算、減算
	Vector2 operator+ (const Vector2& rval)const
	{
		return { x + rval.x, y + rval.y };
	}
	Vector2 operator- (const Vector2& rval)const
	{
		return { x - rval.x, y - rval.y };
	}
	void operator+= (const Vector2& rval);
	void operator-= (const Vector2& rval);

	// スカラー倍、スカラー割り
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;
	void operator*=(float scale);
	void operator/=(float div);

	// ベクター逆転(-)
	Vector2 operator-() const;

	/// ベクトルの大きさを返す
	float Length() const;
	 
	// ベクトルの大きさの2乗を返す
	float SQLength() const;

	// ベクトルの正規化
	void Normalize();

	// 正規化したベクトルを返す
	Vector2 Normalized() const;
};

// 座標を表す別名
// Vector2をPozition2という名前で使う
using Position2 = Vector2;

// サイズをまとめる構造体
struct Size
{	
	// 幅
	int w;

	// 高さ
	int h;
};

/// <summary>
/// 矩形構造体
/// </summary>
struct Rect
{
	Position2 center;
	Size size;

	Rect();
	Rect(const Position2& c, const Size& sz);

	/// <summary>
	/// 左上上右下を指定することで矩形を構築
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="right">右</param>
	/// <param name="bottom">下</param>
	void SetLTRB(int left, int top, int right, int bottom);

	// 矩形の側の座標を返す
	int Left()const;	// 左
	int Top()const;		// 上
	int Right()const;	// 右
	int Bottom()const;	// 下

	// 矩形の座標の取得
	const Position2& GetCenter()const;

	// 矩形の大きさの取得
	const Size& GetSize()const;

	/// <summary>
	/// デバッグ用の矩形表示
	/// </summary>
	/// <param name="Color">表示する矩形の色</param>
	void Draw(unsigned int color = 0xffffff);

	/// <summary>
	/// 別の矩形と当たったかどうか
	/// </summary>
	/// <param name="rc">矩形</param>
	// <returns>true 当たった false 外れた</returns>
	bool IsHit(const Rect& rc)const;
};