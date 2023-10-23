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
	LoadEffectFile(EffectID::player_dead.c_str());	
	LoadEffectFile(EffectID::player_boost.c_str());		
	LoadEffectFile(EffectID::player_acceleration.c_str());	
	LoadEffectFile(EffectID::meteor.c_str());		
}

// 更新
void Effekseer3DEffectManager::Update()
{
	for (auto& effect : effectDataTable_)
	{
		// 再生タイプがループしか通らない
		if (effect.type != PlayType::LOOP) continue;

		// 再生中じゃない場合しか通らない
		if (IsPlayingEffect(*effect.playingEffectHandle)) continue;

		// エフェクトの再再生
		PlayEffectLoop(*effect.playingEffectHandle, effect.effectFileName);

		// エフェクトのパラメーターの設定
		SetEffectAllParam(*effect.playingEffectHandle, effect.pos, effect.scale, effect.speed, effect.rot);
	}
	for (auto& effect : followEffectDataTable_)
	{
		// 再生タイプがループか
		bool isLoop = effect.type == PlayType::LOOP_AND_FOLLOW;

		// 再生中か
		bool isPlay = IsPlayingEffect(*effect.playingEffectHandle);

		// 再生タイプがループ、再生中ではない場合
		if (isLoop && !isPlay)
		{
			// エフェクトの再再生
			PlayEffectLoop(*effect.playingEffectHandle, effect.effectFileName);
		}

		// 再生中の場合エフェクトのパラメーターの更新
		if (isPlay)
		{
			SetEffectAllParam(*effect.playingEffectHandle, *effect.pos, effect.scale, effect.speed, effect.rot);
		}
	}

	// 再生が終了したエフェクトがあった場合削除
	effectDataTable_.remove_if([this](EffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });
	followEffectDataTable_.remove_if([this](FollowEffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });

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

	// エフェクトリソースを削除する(Effekseer終了時に破棄されるので削除しなくてもいいが一応)
	for (auto& effectResource : effectResourceHandleTable_)
	{
		DeleteEffekseerEffect(effectResource.second);
	}

	// 配列の削除
	effectResourceHandleTable_.clear();
	effectDataTable_.clear();
	followEffectDataTable_.clear();

	// Effekseerを終了する
	Effkseer_End();
}

// エフェクトのロード
void Effekseer3DEffectManager::LoadEffectFile(std::string fileName)
{
	// エフェクトのファイルパスの生成
	std::string path = data_file_path;
	path += fileName;
	path += data_extension;

	// エフェクトのロード(失敗したら止める)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1以外じゃなかったら止める

	// ロードしたエフェクトのハンドルの保存
	effectResourceHandleTable_[fileName] = handle;
}

// ループエフェクトの再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, std::string fileName)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// エフェクトの再再生
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);
}

// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.type = PlayType::NORMAL;
	effectDataTable_.push_back(effect);
}

// 指定のエフェクトのループ再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP;
	effectDataTable_.push_back(effect);
}

// 指定のエフェクトの追従再生
void Effekseer3DEffectManager::PlayEffectFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale, float speed, VECTOR rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::FOLLOW;
	followEffectDataTable_.push_back(effect);
}

// 指定のエフェクトの追従とループ再生
void Effekseer3DEffectManager::PlayEffectLoopAndFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale, float speed, VECTOR rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(effectResourceHandleTable_.find(fileName) != effectResourceHandleTable_.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandleTable_[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP_AND_FOLLOW;
	followEffectDataTable_.push_back(effect);
}

// エフェクトの全ての値の変更
void Effekseer3DEffectManager::SetEffectAllParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot)
{
	// 再生速度
	SetEffectSpeed(playingEffectHandle, speed);

	// 拡大率
	SetEffectScale(playingEffectHandle, scale);

	// 回転率
	SetEffectRot(playingEffectHandle, rot);

	// 位置
	SetEffectPos(playingEffectHandle, pos);
}

// エフェクトの拡大率の変更
void Effekseer3DEffectManager::SetEffectScale(int playingEffectHandle, float scale)
{
	// エフェクトの拡大率の設定
	int result = SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale, scale, scale);

	// 失敗したら止める
	assert(result != -1);
	
	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : effectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.scale = scale;
		}
	}
	for (auto& effect : followEffectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.scale = scale;
		}
	}
}

// エフェクトの再生速度の変更
void Effekseer3DEffectManager::SetEffectSpeed(int playingEffectHandle, float speed)
{
	// エフェクトの再生速度の設定
	int result = SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : effectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.speed = speed;
		}
	}
	for (auto& effect : followEffectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.speed = speed;
		}
	}
}

// エフェクトの回転率の変更
void Effekseer3DEffectManager::SetEffectRot(int playingEffectHandle, VECTOR rot)
{
	// エフェクトの回転率の設定
	int result = SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : effectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.rot = rot;
		}
	}
	for (auto& effect : followEffectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.rot = rot;
		}
	}
}

// エフェクトの位置の変更
void Effekseer3DEffectManager::SetEffectPos(int playingEffectHandle, VECTOR pos)
{
	// エフェクトの位置の設定
	int result = SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// 追従エフェクトは必要ないので変更しない
	for (auto& effect : effectDataTable_)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.pos = pos;
		}
	}
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

// 特定のエフェクトの再生をストップ
void Effekseer3DEffectManager::StopEffect(int effectPlayingHandle)
{
	int result = StopEffekseer3DEffect(effectPlayingHandle);
	assert(result != -1);
}

// エフェクト全ての再生をストップ
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : effectDataTable_)
	{
		StopEffect(*effect.playingEffectHandle);
	}
	for (auto& effect : followEffectDataTable_)
	{
		StopEffect(*effect.playingEffectHandle);
	}
}

// 特定のエフェクトの削除
void Effekseer3DEffectManager::DeleteEffect(int effectPlayingHandle)
{
	// エフェクトの再生を止める
	StopEffect(effectPlayingHandle);

	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認しハンドルが一致したデータを削除
	effectDataTable_.remove_if([effectPlayingHandle](EffectData data) { return *data.playingEffectHandle == effectPlayingHandle; });
	followEffectDataTable_.remove_if([effectPlayingHandle](FollowEffectData data) { return *data.playingEffectHandle == effectPlayingHandle; });
}

// 全てのエフェクトの削除
void Effekseer3DEffectManager::DeleteAllEffect()
{
	// 全てのエフェクトの再生を止める
	StopAllEffect();

	// 全てのデータの削除
	effectDataTable_.clear();
	followEffectDataTable_.clear();
}