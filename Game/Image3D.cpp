#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <cassert>

Image3D::Image3D(std::string filePath):
	pos_({}),
	rot_({}),
	imgWidth_(0),
	imgHeight_(0),
	vectex_({})
{
	imgHandle_ = my::MyLoadGraph(filePath.c_str());
}

Image3D::Image3D(int imgHandle):
	imgHandle_(imgHandle),
	pos_({}),
	rot_({}),
	imgWidth_(0),
	imgHeight_(0),
	vectex_({})
{
	assert(imgHandle_ != -1);
}

Image3D::~Image3D()
{
	DeleteGraph(imgHandle_);
}

void Image3D::Update()
{
	vectex_[0].dif = GetColorU8(255, 255, 255, 255);
	vectex_[0].spc = GetColorU8(0, 0, 0, 0);
	vectex_[0].u = 0.0f;
	vectex_[0].v = 0.0f;
	vectex_[0].su = 0.0f;
	vectex_[0].sv = 0.0f;

	vectex_[1].dif = GetColorU8(255, 255, 255, 255);
	vectex_[1].spc = GetColorU8(0, 0, 0, 0);
	vectex_[1].u = 1.0f;
	vectex_[1].v = 0.0f;
	vectex_[1].su = 0.0f;
	vectex_[1].sv = 0.0f;

	vectex_[2].dif = GetColorU8(255, 255, 255, 255);
	vectex_[2].spc = GetColorU8(0, 0, 0, 0);
	vectex_[2].u = 0.0f;
	vectex_[2].v = 1.0f;
	vectex_[2].su = 0.0f;
	vectex_[2].sv = 0.0f;

	vectex_[3].dif = GetColorU8(255, 255, 255, 255);
	vectex_[3].spc = GetColorU8(0, 0, 0, 0);
	vectex_[3].u = 1.0f;
	vectex_[3].v = 1.0f;
	vectex_[3].su = 0.0f;
	vectex_[3].sv = 0.0f;

	vectex_[4] = vectex_[2];
	vectex_[5] = vectex_[1];

	// ��]( x, y, z����]�̏� )�{���W�ړ��s��̍쐬
	MATRIX transformMatrix;
	transformMatrix = MGetRotX(rot_.x);
	transformMatrix = MMult(transformMatrix, MGetRotY(rot_.y));
	transformMatrix = MMult(transformMatrix, MGetRotZ(rot_.z));
	transformMatrix = MMult(transformMatrix, MGetTranslate(VGet(pos_.x, pos_.y, pos_.z)));

	// �s����g���ă��[���h���W���Z�o
	vectex_[0].pos = VTransform(VGet(-imgWidth_, imgHeight_, 0.0f), transformMatrix);	// ����
	vectex_[1].pos = VTransform(VGet(imgWidth_, imgHeight_, 0.0f), transformMatrix);	// �E��
	vectex_[2].pos = VTransform(VGet(-imgWidth_, -imgHeight_, 0.0f), transformMatrix);	// ����
	vectex_[3].pos = VTransform(VGet(imgWidth_, -imgHeight_, 0.0f), transformMatrix);	// �E��

	vectex_[4].pos = vectex_[2].pos;
	vectex_[5].pos = vectex_[1].pos;

	// �s����g���Ė@�����Z�o
	vectex_[0].norm = VTransformSR(VGet(0.0f, 0.0f, -1.0f), transformMatrix);
	vectex_[1].norm = vectex_[0].norm;
	vectex_[2].norm = vectex_[0].norm;
	vectex_[3].norm = vectex_[0].norm;
	vectex_[4].norm = vectex_[0].norm;
	vectex_[5].norm = vectex_[0].norm;
}

void Image3D::Draw()
{
	// �|���S���̗������`�悷��
	SetUseBackCulling(FALSE);

	// ���C�e�B���O�͍s��Ȃ�
	SetUseLighting(FALSE);

	// �Q�|���S���̕`��
	DrawPolygon3D(vectex_.data(), 2, imgHandle_, true);

	// ���C�e�B���O�͍s��Ȃ�
	SetUseLighting(TRUE);

	// �|���S���̗����͕`�悵�Ȃ�
	SetUseBackCulling(TRUE);
}

int Image3D::GetImageHandle() const
{
	return imgHandle_;
}

std::array<VERTEX3D, 6> Image3D::GetVertex() const
{
	return vectex_;
}

void Image3D::SetPos(VECTOR pos)
{
	pos_ = pos;
}

void Image3D::SetRot(VECTOR rot)
{
	rot_ = rot;
}

void Image3D::SetImgWidth(float width)
{
	imgWidth_ = width;
}

void Image3D::SetImgHeight(float height)
{
	imgHeight_ = height;
}

void Image3D::SetImgDafualtScale()
{
	// �摜�̑傫���ǂ���摜��`�悷��
	GetGraphSizeF(imgHandle_, &imgWidth_, &imgHeight_);
}