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
	/// ÉåÅ[ÉUÅ[Çî≠éÀ
	/// </summary>
	/// <param name="pos">î≠éÀèÍèä</param>
	void Fire(const VECTOR pos, const VECTOR vec);

	void Refrect();

	bool GetIsEnabled() const;

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	int modelHandle_;
	bool isEnabled_;
};