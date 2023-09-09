#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace
{
	// エフェクトのファイルパス
	const string data_file_path = "Data/Effect/";

	// エフェクトの拡張子
	const string data_extension = ".efk";
}

/// <summary>
/// コンストラクタ
/// </summary>
Effekseer3DEffectManager::Effekseer3DEffectManager():
	imgHandle_(-1)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Effekseer3DEffectManager::~Effekseer3DEffectManager()
{
}

/// <summary>
/// Effekseer3DEffectManagerの唯一のインスタンスを返す
/// </summary>
/// <returns>唯一の実態の参照</returns>
Effekseer3DEffectManager& Effekseer3DEffectManager::GetInstance()
{
	// 唯一の実態
	static Effekseer3DEffectManager instance;

	// 唯一の実態参照を返す
	return instance;
}

/// <summary>
/// Effekseerの初期化とエフェクトのロード
/// </summary>
/// <returns> 0 : 初期化の成功、-1 : 初期化の失敗  </returns>
int Effekseer3DEffectManager::Init()
{
	// Effekseerを初期化する
	// 引数には画面に表示する最大パーティクル数を設定する
	if (Effkseer_Init(8000) == -1)
	{
		// 初期化の失敗
		return -1;
	}

	// 画像のロード
	imgHandle_ = my::MyLoadGraph("Data/Effect/background.png");

	// ここから↓でEffectをロードする


	// 初期化の成功
	return 0;
}

/// <summary>
/// 毎フレーム更新処理
/// </summary>
void Effekseer3DEffectManager::Update()
{
	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void Effekseer3DEffectManager::Draw()
{
	// 何でもいいので画像を描画する
	// こうして描画した後でないと、Effekseerは描画できない
	DrawGraph(0, 0, imgHandle_, true);

	// Effekseerにより再生中のエフェクトを描画する 
	DrawEffekseer3D();
}

/// <summary>
/// 終了処理
/// </summary>
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

/// <summary>
/// エフェクトのロード
/// </summary>
/// <param name="fileName">ロードしたいエフェクトのファイル名(拡張子は含まない)</param>
void Effekseer3DEffectManager::LoadEffectFile(string fileName)
{
	string path = data_file_path;
	path += fileName;
	path += data_extension;

	// エフェクトのロード(失敗したら止める)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1以外じゃなかったら止める

	// ロードしたエフェクトのハンドルの保存
	effectResourceNameAndHandleTable_[fileName] = handle;
}

/// <summary>
/// 指定のエフェクトの再生
/// </summary>
/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
/// <param name="pos">位置</param>
/// <param name="scale">拡大率</param>
/// <param name="speed">再生速度</param>
/// <param name="rot">回転</param>
void Effekseer3DEffectManager::PlayEffect(string fileName, VECTOR pos, VECTOR scale, float speed, VECTOR rot)
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

/// <summary>
/// 特定のエフェクトが再生中か
/// </summary>
/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
/// <returns>true : 再生中、false : 再生していない</returns>
bool Effekseer3DEffectManager::IsPlayingEffect(string fileName)
{
	if (IsEffekseer3DEffectPlaying(playingEffectNameAndHandleTable_[fileName]) == 0)
	{
		return true;
	}
	return false;
}

/// <summary>
/// エフェクト全ての再生をストップ
/// </summary>
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : playingEffectNameAndHandleTable_)
	{
		StopEffekseer3DEffect(effect.second);
	}
}
