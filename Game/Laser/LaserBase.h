#pragma once
#include <DxLib.h>
#include <memory>
#include "../Util/Model.h"

enum class LaserType
{
	NORMAL, 
	NORMAL_INFINITY,
	CUBE,
	REFLECT
};

class LaserBase
{
public:
	LaserBase();
	virtual ~LaserBase();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ÉJÉÅÉâì‡Ç…ë∂ç›Ç∑ÇÈÇ©
	// ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁçÌèúÇ∑ÇÈ
	virtual void ConfirmDelete() {}

	bool IsEnabled() const;

	int GetModelHandle() const;

	virtual VECTOR GetVec() const;

	bool IsReflect() const;
	void SetIsReflect(bool isReflect);

protected:
	std::unique_ptr<Model> pModel_;

	VECTOR pos_;
	VECTOR* firePos_;
	VECTOR vec_;
	VECTOR rot_;
	VECTOR scale_;

	bool isReflect_;
	bool isEnabled_;
};