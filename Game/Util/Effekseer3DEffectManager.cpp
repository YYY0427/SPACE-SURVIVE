#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "DrawFunctions.h"
#include <cassert>

namespace EffectID
{
	const std::string player_dead = "explosion2";				// プレイヤー死亡時に出すエフェクト
	const std::string player_boost = "starFire";				// プレイヤーブースト時に継続的に出すエフェクト
	const std::string player_acceleration = "acceleration";		// プレイヤーブースト時に一度だけ出すエフェクト
	const std::string meteor = "boost";							// 隕石用エフェクト
}

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
//	auto manager = ::Effekseer::Manager::Create(3000);

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
	LoadEffectFile(EffectID::player_dead.c_str());	
	LoadEffectFile(EffectID::player_boost.c_str());		
	LoadEffectFile(EffectID::player_acceleration.c_str());	
	LoadEffectFile(EffectID::meteor.c_str());		
}

// 更新
void Effekseer3DEffectManager::Update()
{
	// 追従エフェクトの場合の処理
	for (auto& effect : effectDataTable_)
	{
		// 再生中しか通らない
		if (!IsPlayingEffect(*effect.playingEffectHandle)) continue;
		if (effect.type == PlayType::FOLLOW || effect.type == PlayType::LOOP_AND_FOLLOW)
		{
			SetEffectParam(*effect.playingEffectHandle, *effect.pos, *effect.scale, *effect.speed, *effect.rot);
		}
	}
	// ループエフェクトの場合の処理
	for (auto& effect : effectDataTable_)
	{
		// 再生していないときしか通らない
		if (IsPlayingEffect(*effect.playingEffectHandle)) continue;
		if (effect.type == PlayType::LOOP || effect.type == PlayType::LOOP_AND_FOLLOW)
		{
			PlayEffect(effect.playingEffectHandle, effect.effectFileName, effect.type, effect.pos, effect.scale, effect.speed, effect.rot);
		}
	}
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
	for (auto& effectResource : effectResourceHandleTable_)
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
	effectResourceHandleTable_[fileName] = handle;
}

// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(EffectData data)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(data.effectFileName) != effectResourceHandleTable_.end());

	EffectData data2{};
	data2.effectFileName = data.effectFileName;
	*data.playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[data.effectFileName]);
	data2.playingEffectHandle = data.playingEffectHandle;
	data2.type = data.type;
	data2.pos = data.pos;
	data2.rot = data.rot;
	data2.scale = data.scale;
	data2.speed = data.speed;
	effectDataTable_.push_back(data2);

	SetEffectParam(*data2.playingEffectHandle, *data2.pos, *data2.scale, *data2.speed, *data2.rot);
}


// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(int* playingEffectHandle, std::string fileName, PlayType type, VECTOR* pos, float* scale, float* speed, VECTOR* rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	EffectData data{};
	data.effectFileName = fileName;
	*playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);
	data.playingEffectHandle = playingEffectHandle;
	data.type = type;
	data.pos = pos;
	data.rot = rot;
	data.scale = scale;
	data.speed = speed;
	effectDataTable_.push_back(data);

	SetEffectParam(*data.playingEffectHandle, *data.pos, *data.scale, *data.speed, *data.rot);
}

void Effekseer3DEffectManager::SetEffectParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// エフェクトの再生速度を設定
	SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);

	// エフェクトの拡大率の設定
	SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale, scale, scale);

	// エフェクトの回転率の設定
	SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);

	// エフェクトの位置の設定
	SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);
}

// 特定のエフェクトが再生中か
bool Effekseer3DEffectManager::IsPlayingEffect(int effectPlayingHandle)
{
	if (IsEffekseer3DEffectPlaying(effectPlayingHandle) == 0)
	{
		return true;
	}
	return false;
}

void Effekseer3DEffectManager::StopEffect(int effectPlayingHandle)
{
	StopEffekseer3DEffect(effectPlayingHandle);
	
	effectDataTable_.remove_if([effectPlayingHandle](EffectData data) { return *data.playingEffectHandle == effectPlayingHandle; });
}

// エフェクト全ての再生をストップ
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : effectDataTable_)
	{
		StopEffekseer3DEffect(*effect.playingEffectHandle);
	}
	effectDataTable_.clear();
}