#pragma once
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include <list>

// エフェクトのファイルパス
namespace EffectID
{
	const std::string player_dead = "explosion2";				// プレイヤー死亡時に出すエフェクト
	const std::string player_boost = "starFire";				// プレイヤーブースト時に継続的に出すエフェクト
	const std::string player_acceleration = "acceleration";		// プレイヤーブースト時に一度だけ出すエフェクト
	const std::string meteor = "boost";							// 隕石用エフェクト
}

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
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffect(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// 指定のエフェクトのループ再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectLoop(int& playingEffectHandle, std::string fileName, VECTOR pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// 指定のエフェクトの追従再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <param name="pos">追従したい位置情報変数のアドレス</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// 指定のエフェクトの追従とループ再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="fileName">再生したいエフェクトのファイル名(拡張子は含まない)</param>
	/// <param name="pos">追従したい位置情報変数のアドレス</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectLoopAndFollow(int& playingEffectHandle, std::string fileName, VECTOR* pos, float scale = 1.0f, float speed = 1.0f, VECTOR rot = { 0.0f, 0.0f, 0.0f });

	/// <summary>
	/// エフェクトの全ての値の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void SetEffectAllParam(int playingEffectHandle, VECTOR pos, float scale, float speed, VECTOR rot);

	/// <summary>
	/// エフェクトの拡大率の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="scale">拡大率</param>
	void SetEffectScale(int playingEffectHandle, float scale);

	/// <summary>
	/// エフェクトの再生速度の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="speed">再生速度</param>
	void SetEffectSpeed(int playingEffectHandle, float speed);

	/// <summary>
	/// エフェクトの回転率の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="rot">回転率</param>
	void SetEffectRot(int playingEffectHandle, VECTOR rot);

	/// <summary>
	/// エフェクトの位置の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="pos">位置</param>
	void SetEffectPos(int playingEffectHandle, VECTOR pos);

	/// <summary>
	/// 特定のエフェクトが再生中か
	/// </summary>
	/// <param name="fileName">エフェクトのハンドル</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool IsPlayingEffect(int effectPlayingHandle);

	/// <summary>
	/// 特定のエフェクトの再生をストップ
	/// 再生タイプがループのエフェクトの場合は止まった後、次のフレームで再生される
	/// </summary>
	/// <param name="fileName">エフェクトのハンドル</param>
	void StopEffect(int effectPlayingHandle);

	// エフェクト全ての再生をストップ
	// 再生タイプがループのエフェクトの場合は止まった後、次のフレームで再生される
	void StopAllEffect();

	/// <summary>
	/// 特定のエフェクトの削除
	/// 保存しているデータテーブルから削除する
	/// 主に再生タイプがループのエフェクトを削除するのに使う
	/// </summary>
	/// <param name="effectPlayingHandle">エフェクトのハンドル</param>
	void DeleteEffect(int effectPlayingHandle);

	// 全てのエフェクトの削除
	void DeleteAllEffect();

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

	/// <summary>
	/// ループエフェクトの再再生
	/// ループエフェクトの再生が終了していたらエフェクトを再生する
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="fileName">再再生するエフェクトのファイル名(拡張子は含まない)</param>
	void PlayEffectLoop(int& playingEffectHandle, std::string fileName);

private:
	// エフェクトの再生タイプ
	enum class PlayType
	{
		NORMAL,
		FOLLOW,
		LOOP,
		LOOP_AND_FOLLOW,
	};

private:
	// エフェクトデータ
	struct EffectData
	{
		std::string effectFileName;
		int* playingEffectHandle;
		PlayType type;
		VECTOR pos;
		VECTOR rot;
		float scale;
		float speed;
	};

	// 追従エフェクトデータ
	struct FollowEffectData
	{
		std::string effectFileName;
		int* playingEffectHandle;
		PlayType type;
		VECTOR* pos;
		VECTOR rot;
		float scale;
		float speed;
	};

private:
	// エフェクトのファイル名をIDとしたエフェクトリソースのハンドルのテーブル
	std::unordered_map<std::string, int> effectResourceHandleTable_;

	// エフェクトのデータを保存したテーブル
	std::list<EffectData> effectDataTable_;

	// 追従エフェクトのデータを保存したテーブル
	std::list<FollowEffectData> followEffectDataTable_;

	// 画像ハンドル
	int imgHandle_;
};