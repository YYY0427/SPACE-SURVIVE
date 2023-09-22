#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>

// Effekseerを使った3Dエフェクトの管理を行うシングルトンクラス
class Effekseer3DEffectManager
{
public:
	// デストラクタ
	~Effekseer3DEffectManager();

	/// <summary>
	/// Effekseer3DEffectManagerの唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static Effekseer3DEffectManager& GetInstance();

	// Effekseerの初期化とエフェクトのロード
	// 初期化に失敗したら止める
	void Init();

	// 更新
	void Update();

	// 描画
	void Draw();

	// 終了処理
	void End();

	/// <summary>
	/// 指定のエフェクトの再生
	/// </summary>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転</param>
	void PlayEffect(std::string fileName, VECTOR pos, float scale, float speed, VECTOR rot = VGet(0, 0, 0));

	void SetPosPlayingEffect(std::string fileName, VECTOR pos);
	void SetScalePlayingEffect(std::string fileName, float  scale);
	void SetSpeedPlayingEffect(std::string fileName, float speed);
	void SetRotPlayingEffect(std::string fileName, VECTOR rot);

	/// <summary>
	/// 特定のエフェクトが再生中か
	/// </summary>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool IsPlayingEffect(std::string fileName);

	/// <summary>
	/// 特定のエフェクトの再生をストップ
	/// </summary>
	/// <param name="fileName">ストップしたいエフェクトのファイル名(拡張子は含まない)</param>
	void StopEffect(std::string fileName);

	// エフェクト全ての再生をストップ
	void StopAllEffect();

private:
	// コンストラクタ(シングルトンパターンなのでprivateに置く)
	Effekseer3DEffectManager();

	// コピーも代入も禁止する
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// コピーコンストラクタの禁止
	void operator = (const Effekseer3DEffectManager&) = delete;			// 代入の禁止

	/// <summary>
	/// エフェクトのロード
	/// </summary>
	/// <param name="fileName">ロードしたいエフェクトのファイル名(拡張子は含まない)</param>
	void LoadEffectFile(std::string fileName);

private:
	// ロードしたエフェクトのファイル名とハンドル
	std::unordered_map<std::string, int> effectResourceNameAndHandleTable_;

	// 再生中のエフェクトのファイル名とハンドル
	std::unordered_map<std::string, int> playingEffectNameAndHandleTable_;

	// 画像ハンドル
	int imgHandle_;
};