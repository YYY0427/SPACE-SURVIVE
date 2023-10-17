#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include <list>

// エフェクトの再生タイプ
enum class PlayType
{
	NORMAL,
	FOLLOW,
	LOOP,
	LOOP_AND_FOLLOW,
};

struct EffectData
{
	std::string effectFileName;
	int* playingEffectHandle;
	PlayType type;
	VECTOR* pos;
	VECTOR* rot;
	float* scale;
	float* speed;
};

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
	/// <param name="isForcePlay">すでに指定されたエフェクトが再生されている場合も再生するか</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転</param>
	void PlayEffect(int* playingEffectHandle, std::string fileName, PlayType type, VECTOR* pos, float* scale, float* speed, VECTOR* rot);
	void PlayEffectLoop(int* playingEffectHandle, std::string fileName, PlayType type, VECTOR* pos, float* scale, float* speed, VECTOR* rot);

	void SetEffectParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot);

	/// <summary>
	/// 特定のエフェクトが再生中か
	/// </summary>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool IsPlayingEffect(int effectPlayingHandle);

	/// <summary>
	/// 特定のエフェクトの再生をストップ
	/// </summary>
	/// <param name="fileName">ストップしたいエフェクトのファイル名(拡張子は含まない)</param>
	void StopEffect(int effectPlayingHandle);

	// エフェクト全ての再生をストップ
	void StopAllEffect();

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	Effekseer3DEffectManager();

	// コピーも代入も禁止
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	// コピーコンストラクタ禁止
	void operator = (const Effekseer3DEffectManager&) = delete;			// 代入禁止

	/// <summary>
	/// エフェクトのロード
	/// </summary>
	/// <param name="fileName">ロードしたいエフェクトのファイル名(拡張子は含まない)</param>
	void LoadEffectFile(std::string fileName);

private:
	// エフェクトのファイル名をIDとしたエフェクトリソースのハンドルのテーブル
	std::unordered_map<std::string, int> effectResourceHandleTable_;

	// エフェクトのデータを保存した配列
	std::list<EffectData> effectDataTable_;

	// 画像ハンドル
	int imgHandle_;
};