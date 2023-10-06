#include "Imge3D.h"
#include "Util/DrawFunctions.h"
#include <string>

namespace
{
	
}

Imge3D::Imge3D(int imgHandle, VECTOR pos, VECTOR rot)
{
	imgHandle_ = imgHandle;
	pos_ = pos;
	rot_ = rot;

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

	MATRIX transformMatrix;

	// 回転( x, y, z軸回転の順 )＋座標移動行列の作成
	transformMatrix = MGetRotX(rot_.x);
	transformMatrix = MMult(transformMatrix, MGetRotY(rot_.y));
	transformMatrix = MMult(transformMatrix, MGetRotZ(rot_.z));
	transformMatrix = MMult(transformMatrix, MGetTranslate(VGet(pos.x, pos.y, pos.z)));

	// 行列を使ってワールド座標を算出
	vectex_[0].pos = VTransform(VGet(-100.0f, 100.0f, 0.0f), transformMatrix);
	vectex_[1].pos = VTransform(VGet(100.0f, 100.0f, 0.0f), transformMatrix);
	vectex_[2].pos = VTransform(VGet(-100.0f, -100.0f, 0.0f), transformMatrix);
	vectex_[3].pos = VTransform(VGet(100.0f, -100.0f, 0.0f), transformMatrix);

	vectex_[4].pos = vectex_[2].pos;
	vectex_[5].pos = vectex_[1].pos;

	// 行列を使って法線を算出
	vectex_[0].norm = VTransformSR(VGet(0.0f, 0.0f, -1.0f), transformMatrix);
	vectex_[1].norm = vectex_[0].norm;
	vectex_[2].norm = vectex_[0].norm;
	vectex_[3].norm = vectex_[0].norm;
	vectex_[4].norm = vectex_[0].norm;
	vectex_[5].norm = vectex_[0].norm;
}

Imge3D::~Imge3D()
{
	DeleteGraph(imgHandle_);
}

void Imge3D::Draw()
{
	// ライティングは行わない
	SetUseLighting(FALSE);

	// ２ポリゴンの描画
	DrawPolygon3D(vectex_, 2, imgHandle_, true);

	// ライティングは行わない
	SetUseLighting(TRUE);
}