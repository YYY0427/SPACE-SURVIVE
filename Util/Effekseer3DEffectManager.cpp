#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace
{
	// エフェクトのファイルパス
	const std::string data_file_path = "Data/Effect/";

	// エフェクトの拡張子
	const std::string data_extension = ".efk";
}

// コンストラクタ
Effekseer3DEffectManager::Effekseer3DEffectManager():
	imgHandle_(-1)
{
	
}

// デストラクタ
Effekseer3DEffectManager::~Effekseer3DEffectManager()
{
}

// Effekseer3DEffectManagerの唯一のインスタンスを返す
Effekseer3DEffectManager& Effekseer3DEffectManager::GetInstance()
{
	// 唯一の実態
	static Effekseer3DEffectManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// Effekseerの初期化とエフェクトのロード
void Effekseer3DEffectManager::Init()
{
	// Effekseerを初期化する
	// 引数には画面に表示する最大パーティクル数を設定する
	if (Effkseer_Init(8000) == -1)
	{
		// 初期化の失敗
		assert(0);
	}

	// 画像のロード
	imgHandle_ = my::MyLoadGraph("Data/Effect/background.png");

	// ここから↓でEffectをロードする
	LoadEffectFile("explosion2");
}

// 更新
void Effekseer3DEffectManager::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();
}

// 描画
void Effekseer3DEffectManager::Draw()
{
	// 何でもいいので画像を描画する
	// こうして描画した後でないと、Effekseerは描画できない
	DrawGraph(0, 0, imgHandle_, true);

	// Effekseerにより再生中のエフェクトを描画する 
	DrawEffekseer3D();
}

// 終了処理
void Effekseer3DEffectManager::End()
{
	// 画像の削除
	DeleteGraph(imgHandle_);

	// エフェクトリソースを削除する(Effekseer終了時に破棄されるので削除しなくてもいい)
	for (auto& effectResource : effectResourceNameAndHandleTable_)
	{
		DeleteEffekseerEffect(effectResource.second);
	}

	// Effekseerを終了する
	Effkseer_End();
}

// エフェクトのロード
void Effekseer3DEffectManager::LoadEffectFile(std::string fileName)
{
	std::string path = data_file_path;
	path += fileName;
	path += data_extension;

	// エフェクトのロード(失敗したら止める)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1以外じゃなかったら止める

	// ロードしたエフェクトのハンドルの保存
	effectResourceNameAndHandleTable_[fileName] = handle;
}

// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(std::string fileName, VECTOR pos, VECTOR scale, float speed, VECTOR rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceNameAndHandleTable_.find(fileName) != effectResourceNameAndHandleTable_.end());

	// エフェクトの再生(失敗したら止める)
	playingEffectNameAndHandleTable_[fileName] = PlayEffekseer3DEffect(effectResourceNameAndHandleTable_[fileName]);
	assert(playingEffectNameAndHandleTable_[fileName] != -1); // -1以外じゃなかったら止める

	// エフェクトの再生速度を設定
	SetSpeedPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], speed);

	// エフェクトの拡大率の設定
	SetScalePlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], scale.x, scale.y, scale.z);

	// エフェクトの回転率の設定
	SetRotationPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], rot.x, rot.y, rot.z);

	// エフェクトの位置の設定
	SetPosPlayingEffekseer3DEffect(playingEffectNameAndHandleTable_[fileName], pos.x, pos.y, pos.z);
}

// 特定のエフェクトが再生中か
bool Effekseer3DEffectManager::IsPlayingEffect(std::string fileName)
{
	if (IsEffekseer3DEffectPlaying(playingEffectNameAndHandleTable_[fileName]) == 0)
	{
		return true;
	}
	return false;
}

// エフェクト全ての再生をストップ
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : playingEffectNameAndHandleTable_)
	{
		StopEffekseer3DEffect(effect.second);
	}
}
