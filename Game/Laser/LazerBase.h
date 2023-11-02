#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LazerType
{
	NORMAL, 
	CUBE,
};

class LazerBase
{
public:
	LazerBase();
	virtual ~LazerBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// レーザーを発射
	/// </summary>
	/// <param name="pos">発射場所</param>
	virtual void Fire(const VECTOR pos, const VECTOR vec);

	void Delete();
	virtual void Refrect(VECTOR pos){};

	// カメラ内に存在するか
	// 存在しなかったら削除する
	virtual void CheckInCamera() = 0;

	bool GetIsEnabled() const;

	int GetModelHandle() const;

	bool GetIsRefrect() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	bool isEnabled_;

	// 反射後のレーザーか
	bool isRefrect_;
};