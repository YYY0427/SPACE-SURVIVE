#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

using namespace std;

/// <summary>
/// Effekseerを使った3Dエフェクトの管理を行うシングルトンクラス
/// </summary>
class Effekseer3DEffectManager
{
public:
	// デストラクタ
	~Effekseer3DEffectManager();

	// Effekseer3DEffectManagerの唯一のインスタンスを返す
	static Effekseer3DEffectManager& GetInstance();

	// Effekseerの初期化とエフェクトのロード
	int Init();

	// 毎フレーム更新処理
	void Update();

	// 毎フレームの描画処理
	void Draw();

	// 終了処理
	void End();

	// 指定のエフェクトの再生
	void PlayEffect(string fileName, VECTOR pos, VECTOR scale, float speed, VECTOR rot = VGet(0, 0, 0));

	// 特定のエフェクトが再生中か
	bool IsPlayingEffect(string fileName);

	// エフェクト全ての再生をストップ
	void StopAllEffect();

private:
	// コンストラクタ(シングルトンパターンなのでprivateに置く)
	Effekseer3DEffectManager();

	// コピーも代入も禁止する
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// コピーコンストラクタの禁止
	void operator = (const Effekseer3DEffectManager&) = delete;			// 代入の禁止

	// エフェクトのロード
	void LoadEffectFile(string fileName);

private:
	// ロードしたエフェクトのファイル名とハンドル
	unordered_map<string, int> effectResourceNameAndHandleTable_;

	// 再生中のエフェクトのファイル名とハンドル
	unordered_map<string, int> playingEffectNameAndHandleTable_;

	// 画像ハンドル
	int imgHandle_;
};