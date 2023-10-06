#include "Image3D.h"
#include "Util/DrawFunctions.h"
#include <string>

namespace
{
}

Image3D::Image3D(int imgHandle, UnityGameObject data)
{
	imgHandle_ = imgHandle;
	float imgWidth = 0.0f, imgHeight = 0.0f;

#if false
	// 画像の大きさどおり画像を描画する
	GetGraphSizeF(imgHandle_, &imgWidth, &imgHeight);
#else
	// Unityでの表示どおりに画像を描画する
	imgWidth = 500.0f * data.scale.x;
	imgHeight = 500.0f * data.scale.z;
#endif


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

	// 回転( x, y, z軸回転の順 )＋座標移動行列の作成
	MATRIX transformMatrix;
	transformMatrix = MGetRotX(data.rot.x);
	transformMatrix = MMult(transformMatrix, MGetRotY(data.rot.y));
	transformMatrix = MMult(transformMatrix, MGetRotZ(data.rot.z));
	transformMatrix = MMult(transformMatrix, MGetTranslate(VGet(data.pos.x, data.pos.y, data.pos.z)));

	// 行列を使ってワールド座標を算出
	vectex_[0].pos = VTransform(VGet(-imgWidth, imgHeight, 0.0f), transformMatrix);
	vectex_[1].pos = VTransform(VGet(imgWidth, imgHeight, 0.0f), transformMatrix);
	vectex_[2].pos = VTransform(VGet(-imgWidth, -imgHeight, 0.0f), transformMatrix);
	vectex_[3].pos = VTransform(VGet(imgWidth, -imgHeight, 0.0f), transformMatrix);

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

Image3D::~Image3D()
{
}

void Image3D::Draw()
{
	// ライティングは行わない
	SetUseLighting(FALSE);

	// ２ポリゴンの描画
	DrawPolygon3D(vectex_, 2, imgHandle_, true);

	// ライティングは行わない
	SetUseLighting(TRUE);
}